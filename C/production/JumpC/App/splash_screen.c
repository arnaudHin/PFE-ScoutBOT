/**
 * @file splash_screen.c
 * @author Adrien LE ROUX (adrien.leroux@reseau.eseo.fr)
 * @brief 
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
//#include <glib.h>
#include "../watchdog/watchdog.h"
#include "connection_screen.h"
#include "main_screen.h"

static GtkBuilder *p_builder;
static GtkWidget *p_wid_splash;
static Watchdog *wat;

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  FUNCTIONS PROTOTYPES                               ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief display the content of the splashScreen.
 * 
 */
static void display();

/**
 * @brief 
 * 
 */
static void timeout();

/**
 * @brief set the time for the timer ASK_POS_TIME.
 * 
 */
static void setTimeAskPosTime(Watchdog *timer);

/**
 * @brief reset the timer
 * 
 */
static void reset();

/**
 * @brief 
 * 123
 */
static void callback(Watchdog *this);

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   EXTERN FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

extern void splashScreen_new()
{
    p_builder = mainScreen_getBuilder();
    p_wid_splash = mainScreen_getWindows()[3];
    display();
}

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   STATIC FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

static void display()
{
    gtk_widget_show_all(p_wid_splash);
    timeout();
}

static void callback(Watchdog *this)
{
    reset(wat);
    connectionScreen_new();
    gtk_widget_hide(p_wid_splash);
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

static void timeout()
{
    wat = Watchdog_construct(2 * 1000 * 1000, callback);
    setTimeAskPosTime(wat);
}
