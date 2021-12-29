/**
 * @file calibration_screen.c
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
#include "main_screen.h"
#include "notice_popup.h"
#include "../ComJumpC/map_manager.h"

static GtkBuilder *p_builder;
static GtkWidget *p_wid_calibration;
/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  FUNCTIONS PROTOTYPES                               ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief action to calibrate the robot and launch the scan 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_calibrationAction(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief display the content of the calibrationScreen.
 * 
 */
static void display();

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   EXTERN FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

extern void calibrationScreen_new()
{
    p_builder = mainScreen_getBuilder();
    p_wid_calibration = mainScreen_getWindows()[1];
    g_signal_connect(
        gtk_builder_get_object(p_builder, "calibrationAction"),
        "clicked", G_CALLBACK(cb_calibrationAction), NULL);
    display();
}

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   STATIC FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

static void cb_calibrationAction(GtkWidget *p_wid, gpointer p_data)
{
    gtk_widget_hide(p_wid_calibration);
    MapManager_new();
    MapManager_start();
    MapManager_startCalibration();
    //noticePopup_new(CAL_SUCCESS);
}

static void display()
{
    gtk_widget_show_all(p_wid_calibration);
}