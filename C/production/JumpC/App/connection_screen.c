/**
 * @file connection_screen.c
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
#include "../ComJumpC/dispatcher_jump.h"

static GtkBuilder *p_builder;
static GtkWidget *p_wid_connection;

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  FUNCTIONS PROTOTYPES                               ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief display the content of the connectionScreen.
 * 
 */
static void display();

/**
 * @brief action to be connected to the pocket device
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_connection(GtkWidget *p_wid, gpointer p_data);

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   EXTERN FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

extern void connectionScreen_new()
{
    p_builder = mainScreen_getBuilder();
    p_wid_connection = mainScreen_getWindows()[2];

    /* Signal for the connectionButton */
    g_signal_connect(
        gtk_builder_get_object(p_builder, "powerOnOff"),
        "clicked", G_CALLBACK(cb_connection), NULL);
    display();
}

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   STATIC FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

static void display()
{
    gtk_widget_show_all(p_wid_connection);
}

static void cb_connection(GtkWidget *p_wid, gpointer p_data)
{
    dispatcher_jumpC_start();
    gtk_widget_hide(p_wid_connection);
    //noticePopup_new(CON_SUCCESS);
}