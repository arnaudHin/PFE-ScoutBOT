/**
 * @file map_manager.c
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <pthread.h>
#include <mqueue.h>
#include <errno.h>
#include <stdbool.h>
#include "util.h"
#include "map_manager.h"
#include "proxy_cartographer.h"
#include "../Mapping/map_viewer.h"
#include "../watchdog/watchdog.h"

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  TYPEDEF & VARIABLES                                ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @def MQ_MAX_MESSAGES
 *
 * The maximum number of message the queue can handle at one time
 *
 * By default, cannot be higher than 10, unless you change this
 * value into /proc/sys/fs/mqueue/msg_max
 */
#define MQ_MAX_MESSAGES 10

/**
 * @enum mapManager states
 * @brief States of the state machine. MapManager can have the following states: 
 * 
 */
typedef enum
{
    S_FORGET,                      /**< Default state*/
    S_IDLE,                        /**< Idle state*/
    S_WAITING_FOR_POSITIONS,       /**< Waiting for positions state*/
    S_IS_SENDING_TO_MAP_POSITIONS, /**< Is sending positions to map_viewer*/
    S_END_OF_CALIBRATION,          /**< Stop the calibration*/
    S_DEATH,                       /**< State to kill the active object*/
    STATE_NB,                      /**< Allow us to have the number of states*/
} State;

/**
 * @brief Name of the states
 * 
 */
static const char *const stateName[] = {
    "S_FORGET",
    "S_IDLE",
    "S_WAITING_FOR_POSITIONS",
    "S_IS_SENDING_TO_MAP_POSITIONS ",
    "S_END_OF_CALIBRATION",
    "S_DEATH",
    "NB_STATE"};

/**
 * @brief Allow us to get the name of a state
 * 
 * @param i the 'id' of a state in "stateName"
 * @return const char* the name of the state
 */
static const char *stateGetName(int8_t i)
{
    return stateName[i];
}

/**
 * @enum mapManager action
 * @brief Actions of the state machine. MapManager can have the following actions: 
 * 
 */
typedef enum
{
    A_NOP,           /**<Action that does nothing*/
    A_ENDOFCALIB,    /**<Action when E_STOP_CALIBRATION event*/
    A_IDLETOWFP,     /**<Action when E_START_CALIBRATION*/
    A_WFBTOISMAPPOS, /**<Action when E_SET_MAP_POSITIONS*/
    A_ENTERACTIVE,   /**<Action when to enter into the Active super state*/
    A_ISMAPPOSTOWFB, /**<Action when we are passing from S_IS_SENDING_TO_MAP_POSITIONS to S_WAITING_FOR_POSITIONS*/
    ACTION_NB,       /**<Number of actions*/
} Action;

/**
 * @brief Name of the actions
 * 
 */
static const char *const actionName[] = {
    "A_NOP",
    "A_ENDOFCALIB",
    "A_IDLETOWFP",
    "A_WFBTOISMAPPOS",
    "A_ENTERACTIVE",
    "A_ISMAPPOSTOWFB",
    "ACTION_NB"};

/**
 * @brief Allow us to get the name of an action
 * 
 * @param i the 'id' of an action in "actionName"
 * @return const char* the name of the action
 */
static const char *actionGetName(int8_t i)
{
    return actionName[i];
}

/**
 * @enum MapManager events
 * @brief Events of the state machine. MapManager can have the following events:  
 * 
 */
typedef enum
{
    E_START_CALIBRATION, /**<Event to start the calibration*/
    E_SET_MAP_POSITIONS, /**<Event to set map positions*/
    E_TIME_OUT,          /**<Event to ask again map positions*/
    E_STOP_CALIBRATION,  /**<Event to stop the calibration*/
    E_DEATH,             /**<Event to kill the active object*/
    EVENT_NB,            /**<Number of event*/
} Event;

/**
 * @brief Name of the events
 * 
 */
static const char *const eventName[] = {
    "E_START_CALIBRATION",
    "E_SET_MAP_POSITIONS",
    "E_TIME_OUT",
    "E_STOP_CALIBRATION",
    "E_DEATH",
    "EVENT_NB"};

/**
 * @brief Allow us to get the name of an event
 * 
 * @param i the 'id' of an event in "eventName"
 * @return const char* the name of the event
 */
static const char *eventGetName(int8_t i)
{
    return eventName[i];
}

typedef struct
{
    State destinationState;
    Action action;
} Transition;

static Transition *myTrans;
typedef void (*ActionPtr)();
static DATA_from_pocket_t dataPos;
static pthread_t myThread;
static mqd_t myBal;
static const char queueName[] = "/Balou";
static Watchdog *wat;
static bool flag = false;
static uint8_t flag_wat = 0;

typedef struct
{
    Event event;
    // generic definition to copy events' parameters
} MqMsgData;

/**
 * @brief Enable to send and receive messages without state convertion concern
 * 
 */
typedef union
{
    MqMsgData data;
    char mqBuffer[sizeof(MqMsgData)];
} MqMsg;

/**
 * @brief Current State, initialize at S_IDLE
 * 
 */
static State myState = S_IDLE;

/**
 * @brief State Machine of MapManager
 * 
 */
static Transition mySm[STATE_NB][EVENT_NB] = {
    //Transitions into Active super state
    [S_IDLE][E_START_CALIBRATION] = {S_WAITING_FOR_POSITIONS, A_IDLETOWFP},
    [S_WAITING_FOR_POSITIONS][E_SET_MAP_POSITIONS] = {S_IS_SENDING_TO_MAP_POSITIONS, A_WFBTOISMAPPOS},
    [S_IS_SENDING_TO_MAP_POSITIONS][E_TIME_OUT] = {S_WAITING_FOR_POSITIONS, A_ISMAPPOSTOWFB},

    //From Active to EndOfCalibration
    [S_WAITING_FOR_POSITIONS][E_STOP_CALIBRATION] = {S_END_OF_CALIBRATION, A_ENDOFCALIB},
    [S_IS_SENDING_TO_MAP_POSITIONS][E_STOP_CALIBRATION] = {S_END_OF_CALIBRATION, A_ENDOFCALIB},

    //From EndTour to Active
    [S_END_OF_CALIBRATION][E_START_CALIBRATION] = {S_WAITING_FOR_POSITIONS, A_IDLETOWFP},

};

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  FUNCTIONS PROTOTYPES                               ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Actions while A_ENTERACTIVE. 
 * 
 */
static void actionEnterActive(void);

/**
 * @brief No action => While Action A_NOP set
 * 
 */
static void actionNop(void);

/**
 * @brief Actions while A_ENDOFCALIBRATION set
 * 
 */
static void actionEndOfCalibration(void);

/**
 * @brief action while A_IDLETOWFP set
 * 
 */
static void actionIdleToWfp(void);

/**
 * @brief action A_WFBTOISMAPPOS set
 * 
 */
static void actionWfpToIsSendMapPositions(void);

/**
 * @brief action A_ISMAPPOSTOWFB set
 * 
 */
static void actionIsSendMapPositionsToWfb(void);

/**
 * @brief Routine of the thread. This while keep alive the state machine
 * 
 */
static void *run(void *aParam);

/**
 * @brief set the time for the timer ASK_POS_TIME.
 * 
 */
static void setTimeAskPosTime(Watchdog *timer);

/**
 * @brief Send a text message on the message queue when the askPos time is done
 * 
 */
static void timeOutAskPosTime();

/**
 * @brief reset the timer
 * 
 */
static void reset();

/**
 * @brief To receive a mqMessage
 * 
 * @param aMsg is a State
 */
static void mqReceive(MqMsg *aMsg);

/**
 * @brief To send a mqMessage
 * 
 * @param aMsg is a State
 */
static void mqSend(MqMsg *aMsg);

/**
 * @brief Table of actions to realize => as a function pointers
 * 
 */
static const ActionPtr actionsTab[ACTION_NB] = {&actionNop, &actionEndOfCalibration, &actionIdleToWfp, &actionWfpToIsSendMapPositions, &actionEnterActive, &actionIsSendMapPositionsToWfb};

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   EXTERN FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

extern void MapManager_new()
{
    int8_t check;
    struct mq_attr attr = {
        .mq_maxmsg = MQ_MAX_MESSAGES,
        .mq_msgsize = sizeof(MqMsg),
        .mq_flags = 0,
    };

    check = mq_unlink(queueName);
    if ((check == -1) && (errno != ENOENT))
    {
        PERROR("Error when unlinking the BAL\n");
    }
    myBal = mq_open(queueName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, &attr);
    if (myBal == -1)
    {
        PERROR("Error when opening the BAL\n");
    }
    myState = S_IDLE;
}

extern void MapManager_setMapPositions(DATA_from_pocket_t data)
{
    MqMsg msg = {.data.event = E_SET_MAP_POSITIONS};
    dataPos.lidarData = data.lidarData;
    dataPos.positionData = data.positionData;
    TRACE("E_SET_MAP_POSITIONS Event\n");
    mqSend(&msg);
}

extern void MapManager_startJumpC()
{
    MqMsg msg = {.data.event = E_START_CALIBRATION};
    TRACE("E_START_CALIBRATION Event\n");
    mqSend(&msg);
}

extern void MapManager_start()
{
    // State machine routine
    int8_t check;

    check = pthread_create(&myThread, NULL, &run, NULL);
    if (check != 0)
    {
        PERROR("Error to the return value of the thread cration\n");
    }
    TRACE("Launching\n");
    pthread_detach(myThread);
}

extern void MapManager_stop()
{
    MqMsg msg = {.data.event = E_DEATH};
    TRACE("Death Event\n");
    mqSend(&msg);
}

extern void MapManager_stopCalibration()
{
    MqMsg msg = {.data.event = E_STOP_CALIBRATION};
    TRACE("E_STOP_CALIBRATION Event\n");
    mqSend(&msg);
}

extern void MapManager_free()
{
    myState = S_FORGET;
    int8_t check;
    check = mq_close(myBal);
    if (check == -1)
    {
        PERROR("Error to the return value of mq_close function\n");
    }
    check = mq_unlink(queueName);
    if (check == -1)
    {
        PERROR("Error to the return value of mq_unlink function\n");
    }
    TRACE("Destruction\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   STATIC FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

void MapManager_transitionFct(MqMsg msg)
{
    myTrans = &mySm[myState][msg.data.event];

    TRACE("\033[33m MAE, events management %s \033[0m\n", eventGetName(msg.data.event));

    if (myTrans->destinationState != S_FORGET)
    {
        actionsTab[myTrans->action](); //execution of the action
        TRACE("\033[33m MAE, actions management %s \033[0m\n", actionGetName(myTrans->action));
        myState = myTrans->destinationState;
        TRACE("\033[33m MAE, going to state %s \033[0m\n", stateGetName(myState));
    }
    else
    {
        TRACE("\033[33mMAE, lost event %s  \033[0m\n", eventGetName(msg.data.event));
    }
}

static void *run(void *aParam)
{
    MqMsg msg;
    while (myState != S_FORGET)
    {
        mqReceive(&msg);
        if (msg.data.event == E_DEATH)
        {
            myState = S_FORGET;
            MapManager_free();
        }
        else
        {
            MapManager_transitionFct(msg);
        }
    }
    return NULL;
}

static void mqReceive(MqMsg *aMsg)
{
    int8_t check;
    check = mq_receive(myBal, (char *)aMsg, sizeof(*aMsg), NULL);
    if (check == -1)
    {
        PERROR("Error to the return value of mq_receive function\n");
    }
}

static void mqSend(MqMsg *aMsg)
{
    int8_t check;
    check = mq_send(myBal, (const char *)aMsg->mqBuffer, sizeof(MqMsg), 0);
    if (check != 0)
    {
        PERROR("Error to the return value of mq_send function\n");
    }
}

static void reset(Watchdog *timer)
{
    Watchdog_cancel(timer);
    Watchdog_destroy(timer);
}

static void setTimeAskPosTime(Watchdog *timer)
{
    Watchdog_start(timer);
}

static void timeOutAskPosTime()
{
    MqMsg msg = {.data.event = E_TIME_OUT};
    TRACE("[MapManager] Time OutPosTime \n");
    mqSend(&msg);
}

static void actionNop()
{
    TRACE("[MapManager] Action NOP \n");
}

static void actionEndOfCalibration()
{
    reset(wat);
    mapViewer_free();
    flag_wat = 0;
    flag = false;
    TRACE("[MapManager] Action EndOfCalibration \n");
    proxyCartographer_signal_stop();
}

static void actionIdleToWfp()
{
    TRACE("[MapManager] Action IdleToWfp \n");
    proxyCartographer_signal_start();
}

static void actionWfpToIsSendMapPositions()
{

    if( flag_wat == 1){
        reset(wat);
    }

    //requestPositionsReceivedSuccessfull();

    if (flag != true)
    {
        mapViewer_calibrationSuccessful();
        flag = true;
        flag_wat = 1;
    }

    mapViewer_setData(dataPos);
    
    
    TRACE("[MapManager] Action WfpToIsSendMapPositions \n");
    wat = Watchdog_construct(1 * 1000 * 1000, timeOutAskPosTime);
    setTimeAskPosTime(wat);


}

static void actionEnterActive()
{
    TRACE("[MapManager] ENTER in IDLE \n");
}

static void actionIsSendMapPositionsToWfb()
{
    proxyCartographer_ask4data();
}