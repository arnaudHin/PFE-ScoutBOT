/**
 * @file notice_popup.c
 * @author Adrien LE ROUX (adrien.leroux@reseau.eseo.fr)
 * @brief 
 * @version 0.1
 * @date 2021-12-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "main_screen.h"
#include "connection_screen.h"
#include "notice_popup.h"

static GtkBuilder *p_builder;
static GtkWidget *p_wid_popup;
static Choice choix;
/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  FUNCTIONS PROTOTYPES                               ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief display the next screen 
 * 
 * @param p_wid 
 * @param p_data 
 * @param choice 
 */
static void cb_popupButtonAction(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief display the content of the noticePopup.
 * 
 */
static void display(Choice choice);

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   EXTERN FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

extern void noticePopup_new(Choice choice)
{
    p_builder = mainScreen_getBuilder();
    p_wid_popup = mainScreen_getWindows()[4];
    g_signal_connect(
        gtk_builder_get_object(p_builder, "acquitteButton"),
        "clicked", G_CALLBACK(cb_popupButtonAction), NULL);
    display(choice);
}

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   STATIC FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

static void cb_popupButtonAction(GtkWidget *p_wid, gpointer p_data)
{
    switch (choix)
    {
    case CAL_SUCCESS:
        mainScreen_new();
        break;
    case CAL_UNSUCCESS:
        mainScreen_new();
        break;
    case CON_SUCCESS:
        mainScreen_new();
        break;
    case CON_UNSUCCESS:
        connectionScreen_new();
        break;
    default:
        break;
    }
    gtk_widget_hide(p_wid_popup);
}

static void display(Choice choice)
{
    switch (choice)
    {
    case CAL_SUCCESS:
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(p_builder, "label1")), "La calibration et le démarrage du scan a été effectué avec succès.\nPressez Ok pour commander le robot.");
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapDynamique")), TRUE);
        break;
    case CAL_UNSUCCESS:
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(p_builder, "label1")), "La calibration n'a pas marché.\nVeuillez réessayer.");
        break;
    case CON_SUCCESS:
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(p_builder, "label1")), "La connexion entre la télécommande et le robot a bien été établie.\nPressez Ok pour commander le robot.");
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapDynamique")), FALSE);
        break;
    case CON_UNSUCCESS:
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(p_builder, "label1")), "La connexion entre la télécommande et le robot n'a pas été établie.\nVeuillez réessayer.");
        break;
    default:
        break;
    }
    choix = choice;
    gtk_widget_show_all(p_wid_popup);
}