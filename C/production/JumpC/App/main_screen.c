/**
 * @file main_screen.c
 * @author Adrien LE ROUX (adrien.leroux@reseau.eseo.fr)
 * @brief 
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <pthread.h>
#include "../ComJumpC/proxy_pilot.h"
#include "../ComJumpC/postman_jump.h"
#include "main_screen.h"
#include "../ComJumpC/util.h"
#include "splash_screen.h"
#include "connection_screen.h"
#include "calibration_screen.h"
#include "../ComJumpC/proxy_cartographer.h"

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  TYPEDEF & VARIABLES                                ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

#define NUMBER_OF_SCREENS (5)
static GtkBuilder *p_builder = NULL;
static DATA_from_pocket_t dataPos;
static GtkWidget **p_wins;
static Mode_A currentMode = STATIC;
static bool startedStatic = false;

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                  FUNCTIONS PROTOTYPES                               ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief lock or dislock buttons on the screen
 * 
 * @param state 
 */
static void switch_block(bool state);

/**
 * @brief 
 * 
 * @param widget 
 * @param x 
 * @param y 
 */
static void draw_brush(GtkWidget *p_wid);

/**
 * @brief 
 * 
 * @param p_wid 
 * @param cr 
 * @param data 
 * @return gboolean 
 */
static gboolean on_draw1_draw(GtkWidget *p_wid, cairo_t *cr, gpointer data);

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_forward(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_left(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_mapStatic(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_mapDynamic(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_moveback(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief 
 * 
 * @param source 
 * @param user_data 
 */
static void output_vocal(GtkToggleButton *source, gpointer user_data);

/**
 * @brief 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_quit(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_right(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_stop(GtkWidget *p_wid, gpointer p_data);

/**
 * @brief 
 * 
 */
static void display();

/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   EXTERN FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

extern GtkBuilder *mainScreen_getBuilder()
{
    return p_builder;
}

extern GtkWidget **mainScreen_getWindows()
{
    return p_wins;
}

extern void mainScreen_new()
{
    display();
}

extern void mainScreen_draw_static_refresh(DATA_from_pocket_t data)
{
    dataPos = data;
    draw_brush(p_wins[0]);
}

extern void mainScreen_draw_dynamic_refresh(Lidar_data_t data)
{
    dataPos.lidarData = data;
    draw_brush(p_wins[0]);
}
/////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                     ////
////                                   STATIC FUNCTIONS                                  ////
////                                                                                     ////
/////////////////////////////////////////////////////////////////////////////////////////////

static void switch_block(bool state)
{
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "leftButton")), state);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "rightButton")), state);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "forwardButton")), state);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "movebackButton")), state);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "stopActionButton")), state);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "calibrationButton")), state);
}

static void draw_brush(GtkWidget *p_wid)
{
    GtkStyleContext *context;
    g_signal_connect(
        gtk_builder_get_object(p_builder, "draw1"),
        "draw", G_CALLBACK(on_draw1_draw), NULL);
    context = gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(p_builder, "box")));
    gtk_style_context_add_class(context, "box");
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(p_builder, "draw1")));
}

static gboolean on_draw1_draw(GtkWidget *p_wid, cairo_t *cr, gpointer data)
{
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 1.0, 1.0, 0.0);
    if (STATIC == currentMode)
    {
        if (startedStatic == false)
        {
            for (int i = 0; i < LIDAR_TOTAL_DEGREE; i++)
            {
                cairo_rectangle(cr, dataPos.lidarData.X_buffer[i], dataPos.lidarData.Y_buffer[i], 2.5, 2.5);
            }
            startedStatic = true;
        }
        cairo_rectangle(cr, dataPos.positionData.x, dataPos.positionData.y, 2.5, 2.5);
    }
    else
    {
        for (int i = 0; i < LIDAR_TOTAL_DEGREE; i++)
        {
            cairo_rectangle(cr, dataPos.lidarData.X_buffer[i], dataPos.lidarData.Y_buffer[i], 2.5, 2.5);
        }
        cairo_stroke(cr);
        cairo_set_source_rgb(cr, 1.0, 0.32, 0.4);
        cairo_rectangle(cr, 380 / 2 - 5, 230 / 2 - 5, 10.0, 10.0); //Middle point - car in the center
    }
    cairo_stroke(cr);

    return FALSE;
}

static void cb_forward(GtkWidget *p_wid, gpointer p_data)
{
    proxy_pilot_set_direction(FORWARD);
}

static void cb_left(GtkWidget *p_wid, gpointer p_data)
{
    proxy_pilot_set_direction(LEFT);
}

static void cb_mapStatic(GtkWidget *p_wid, gpointer p_data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapStatique")), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapDynamique")), TRUE);
    currentMode = STATIC;
    mainScreen_draw_static_refresh(dataPos);
}

static void cb_mapDynamic(GtkWidget *p_wid, gpointer p_data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapDynamique")), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapStatique")), TRUE);
    currentMode = DYNAMIC;
    mainScreen_draw_dynamic_refresh(dataPos.lidarData);
}

static void output_vocal(GtkToggleButton *source, gpointer user_data)
{
    printf("Vocal active: %d\n", gtk_toggle_button_get_active(source));
    if (gtk_toggle_button_get_active(source) == TRUE)
    {
        switch_block(FALSE);
    }
    else
    {
        switch_block(TRUE);
    }
}

static void cb_moveback(GtkWidget *p_wid, gpointer p_data)
{
    proxy_pilot_set_direction(BACKWARD);
}

static void cb_quit(GtkWidget *p_wid, gpointer p_data)
{
    gtk_main_quit();
}

static void cb_right(GtkWidget *p_wid, gpointer p_data)
{
    proxy_pilot_set_direction(RIGHT);
}

static void cb_stop(GtkWidget *p_wid, gpointer p_data)
{
    proxy_pilot_set_direction(STOP);
}

static void display()
{
    gtk_widget_show_all(p_wins[0]);
}

static void output_state(GtkToggleButton *source, gpointer user_data)
{
    printf("Active: %d\n", gtk_toggle_button_get_active(source));
    if (gtk_toggle_button_get_active(source) == TRUE)
    {
        gtk_widget_hide(p_wins[0]);
        calibrationScreen_new();
    }
    else
    {
        proxyCartographer_signal_stop();
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "mapDynamique")), FALSE);
    }
}

int main(int argc, char **argv)
{
    p_wins = malloc(sizeof(GtkWidget *) * NUMBER_OF_SCREENS);

    GError *p_err = NULL;
    GtkCssProvider *cssProvider1;
    cssProvider1 = gtk_css_provider_new();
    //p1 = p2 = start = NULL;
    /* Initialisation de GTK+ */
    gtk_init(&argc, &argv);
    gtk_css_provider_load_from_path(cssProvider1, "Jump_CSS/gtk.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //postman_jumpC_start();
    /* Creation d'un nouveau GtkBuilder */
    //css_set(cssProvider1, GTK_WIDGET(gtk_builder_get_object(p_builder, "window")));

    p_builder = gtk_builder_new();

    if (p_builder != NULL)
    {
        /* Chargement du XML dans p_builder */
        gtk_builder_add_from_file(p_builder, "projetUI.glade", &p_err);
        if (p_err == NULL)
        {
            /* 1.- Recuparation d'un pointeur sur la fenetre. */
            GtkWidget *p_win = (GtkWidget *)gtk_builder_get_object(
                p_builder, "window");
            p_wins[0] = p_win;
            GtkWidget *p_win_calibration = (GtkWidget *)gtk_builder_get_object(
                p_builder, "window1");
            p_wins[1] = p_win_calibration;
            GtkWidget *p_win_connection = (GtkWidget *)gtk_builder_get_object(
                p_builder, "window2");
            p_wins[2] = p_win_connection;
            GtkWidget *p_win_splash = (GtkWidget *)gtk_builder_get_object(
                p_builder, "window3");
            p_wins[3] = p_win_splash;
            GtkWidget *p_win_popup = (GtkWidget *)gtk_builder_get_object(
                p_builder, "popup");
            p_wins[4] = p_win_popup;
            for (int i = 0; i < 4; i++)
            {
                gtk_window_set_default_size(GTK_WINDOW(p_wins[i]), 600, 350);
                gtk_window_set_resizable(GTK_WINDOW(p_wins[i]), FALSE);
                g_signal_connect(
                    p_wins[i],
                    "delete-event", G_CALLBACK(cb_quit), NULL);
            }
            gtk_window_set_default_size(GTK_WINDOW(p_wins[4]), 500, 220);
            gtk_window_set_resizable(GTK_WINDOW(p_wins[4]), FALSE);

            /* Vocal button signal */
            g_signal_connect(
                gtk_builder_get_object(p_builder, "vocaleButton"),
                "toggled", G_CALLBACK(output_vocal), NULL);
            /* Left button signal */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "leftButton"),
                "clicked", G_CALLBACK(cb_left), NULL);

            /* Right button signal */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "rightButton"),
                "clicked", G_CALLBACK(cb_right), NULL);

            /* PowerOnOff button signal */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "powerOnOff"),
                "clicked", G_CALLBACK(cb_mapStatic), NULL);

            /* Foward button signal */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "forwardButton"),
                "clicked", G_CALLBACK(cb_forward), NULL);

            /* Moveback button signal */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "movebackButton"),
                "clicked", G_CALLBACK(cb_moveback), NULL);

            /* Moveback button signal */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "stopActionButton"),
                "clicked", G_CALLBACK(cb_stop), NULL);

            /* Change screen to calibrationScreen */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "calibrationButton"),
                "toggled", G_CALLBACK(output_state), NULL);

            /* Change the static mapScreen mode into a dynamic  */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "mapDynamique"),
                "clicked", G_CALLBACK(cb_mapDynamic), NULL);

            /* Change the static mapScreen mode into a dynamic  */

            g_signal_connect(
                gtk_builder_get_object(p_builder, "mapStatique"),
                "clicked", G_CALLBACK(cb_mapStatic), NULL);

            /* Send a signal to ask position to Cartographer  */

            //gtk_widget_set_events(GTK_WIDGET(gtk_builder_get_object(p_builder, "draw1")), GDK_BUTTON_MOTION_MASK | GDK_BUTTON_PRESS_MASK);
            //css_set(cssProvider1, gtk_builder_get_object(p_builder, "stopButton"));
            //gtk_widget_show_all(p_win);
            //splashScreen_new();
            connectionScreen_new();
            //switch_block(FALSE);
            gtk_main();
        }
        else
        {
            /* Affichage du message d'erreur de GTK+ */
            g_error("%s", p_err->message);
            g_error_free(p_err);
        }
    }

    return EXIT_SUCCESS;
}