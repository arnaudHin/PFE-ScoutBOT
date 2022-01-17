/**
 * @file map_manager.h
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include "commun.h"

/**
 * @brief Start the routine of the thread to run the state machine
 * @see *run()
 * 
 */
extern void MapManager_start(void);

/**
 * @brief Ask pocket to start the calibration
 * 
 */
extern void MapManager_startJumpC(void);

/**
 * @brief Set the sending positions
 * 
 * @param data 
 */
extern void MapManager_setMapPositions(DATA_from_pocket_t data);

/**
 * @brief Kill the active object
 * 
 */
extern void MapManager_stop(void);

/**
 * @brief Stop the calibration process
 * 
 */
extern void MapManager_stopCalibration();

/**
 * @brief Destroys the message queue
 * 
 */
extern void MapManager_free(void);

/**
 * @brief Initializes the message queue
 * 
 */
extern void MapManager_new(void);
