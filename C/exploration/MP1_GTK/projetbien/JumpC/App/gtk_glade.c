#include <stdlib.h>
#include "stdbool.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include "../ComJumpC/proxy_pilot.h"
#include "../ComJumpC/postman_jump.h"
#include "gtk_glade.h"
#include "../ComJumpC/util.h"
/**
 * @brief Current state of the remote (Pause or Play)
 * 
 */

/**
 * @brief lock or dislock buttons on the screen
 * 
 * @param state 
 */
static void switch_block(bool state);

static pthread_mutex_t mutex_amount = PTHREAD_MUTEX_INITIALIZER; //On créé le mutex qu'on intilialise par PTHREAD_MUTEX_INITIALIZER
GtkBuilder *p_builder = NULL;
struct Point *p1, *p2, *start;

GtkWidget *p_win1;
/**
 * @brief 
 * 
 * @param widget 
 * @param x 
 * @param y 
 */
static void draw_brush(GtkWidget *p_wid, gdouble x, gdouble y);

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
 * @brief 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_map(GtkWidget *p_wid, gpointer p_data);

void *consumer(void *param) // FOnction qu'on définira en thread dans la foncion main
{
   int i;

   for (i = 0; i < 12; i++)
   {
      pthread_mutex_lock(&mutex_amount);   // Le mutex bloque l'accès à n'importe quel thread rentrant
                                           // Account_withdraw(WITHDRAW_AMOUNT);
      pthread_mutex_unlock(&mutex_amount); // Le mutex ouvre l'accès aux threads entrants après avoir réaliser l'opération
   }

   return NULL;
}

// static void cb_ok(GtkWidget *p_wid, gpointer p_data)
// {
//    GtkWidget *p_dialog = NULL;
//    const char *p_text = p_data;

//    if (p_text != NULL)
//    {
//       p_dialog = gtk_message_dialog_new(
//           NULL,
//           GTK_DIALOG_MODAL,
//           GTK_MESSAGE_INFO,
//           GTK_BUTTONS_OK,
//           p_text);

//       gtk_dialog_run(GTK_DIALOG(p_dialog));
//       gtk_widget_destroy(p_dialog);
//    }
// }
static void switch_block(bool state)
{
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "pauseButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "buttonPlay")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "leftButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "rightButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "forwardButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "movebackButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "stopActionButton")), state);
}
/**
 * @brief 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_quit(GtkWidget *p_wid, gpointer p_data)
{
   gtk_main_quit();
}

static void draw_brush(GtkWidget *p_wid, gdouble x, gdouble y)
{
   g_signal_connect(
       gtk_builder_get_object(p_builder, "draw1"),
       "draw", G_CALLBACK(on_draw1_draw), NULL);

   if (p1 == NULL)
   {
      TRACE("Out of memory");
      abort();
   }
   // p1->x = x;
   // p1->y = y;
   start = malloc(sizeof(struct Point));
   start->x = p1->next_x;
   start->y = p1->next_y;
   gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(p_builder, "draw1")));
}

static gboolean on_draw1_draw(GtkWidget *p_wid, cairo_t *cr, gpointer data)
{
   guint width, height;

   width = gtk_widget_get_allocated_width(p_wid);
   height = gtk_widget_get_allocated_height(p_wid);

   cairo_set_line_width(cr, 1.0);

   // if (start == NULL)
   //    return FALSE;
   int old_x = start->x;
   int old_y = start->y;

   //p1 = start->next;

   // while (p1 != NULL)
   // {
   cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
   cairo_move_to(cr, (double)old_x, (double)old_y);
   cairo_line_to(cr, (double)p1->x, (double)p1->y);
   cairo_stroke(cr);
   //cairo_stroke_extents ()

   // old_x = p1->x;
   // old_y = p1->y;
   // }
   return FALSE;
}
/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_play(GtkWidget *p_wid, gpointer p_data)
{
   switch_block(TRUE);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "buttonPlay")), FALSE);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_pause(GtkWidget *p_wid, gpointer p_data)
{
   switch_block(FALSE);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "buttonPlay")), TRUE);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_left(GtkWidget *p_wid, gpointer p_data)
{
   proxy_set_direction(LEFT);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_right(GtkWidget *p_wid, gpointer p_data)
{
   proxy_set_direction(RIGHT);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_forward(GtkWidget *p_wid, gpointer p_data)
{
   proxy_set_direction(FORWARD);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_moveback(GtkWidget *p_wid, gpointer p_data)
{
   proxy_set_direction(BACKWARD);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_stop(GtkWidget *p_wid, gpointer p_data)
{
   proxy_set_direction(STOP);
}

/**
 * @brief 
 * 
 * @param p_wid 
 * @param p_data 
 */
static void cb_moveScreen(GtkWidget *p_wid, gpointer p_data)
{
   gtk_main_quit();
   // GError *p_err = NULL;

   // if (p_err == NULL)
   // {
   //    /* 1.- Recuparation d'un pointeur sur la fenetre. */

   //    g_signal_connect(
   //        gtk_builder_get_object(p_builder, "map"),
   //        "clicked", G_CALLBACK(cb_map), NULL);
   // }
   // else
   // {
   //    /* Affichage du message d'erreur de GTK+ */
   //    g_error("%s", p_err->message);
   //    g_error_free(p_err);
   // }
   gtk_widget_show_all(p_win1);
   gtk_main();
}

static void cb_map(GtkWidget *p_wid, gpointer p_data)
{
   p1 = malloc(sizeof(struct Point));
   p1->x = 3;
   p1->y = 8;
   p1->next_x = 50;
   p1->next_y = 80;
   draw_brush(p_wid, p1->x, p1->y);
}
/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_powerOnOff(GtkWidget *p_wid, gpointer p_data)
{
   //gtk_builder_get_objects   for every objects
   if (gtk_switch_get_active(GTK_WIDGET(gtk_builder_get_object(p_builder, "powerOnOff"))) == 0)
   {
      postman_jumpC_stop();
      switch_block(FALSE);
   }
   else
   {
      postman_jumpC_start();
      gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "buttonPlay")), TRUE);
   }
}

extern void on_clear_clicked(GtkWidget *b1)
{
   //p1
}
int main(int argc, char **argv)
{

   GError *p_err = NULL;
   //p1 = p2 = start = NULL;
   /* Initialisation de GTK+ */
   gtk_init(&argc, &argv);
   //postman_jumpC_start();
   /* Creation d'un nouveau GtkBuilder */
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

         p_win1 = (GtkWidget *)gtk_builder_get_object(
             p_builder, "window1");

         // /* 2.- */
         /* Play button signal  */
         g_signal_connect(
             gtk_builder_get_object(p_builder, "buttonPlay"),
             "clicked", G_CALLBACK(cb_play), NULL);

         /* Pause button signal */
         g_signal_connect(
             gtk_builder_get_object(p_builder, "pauseButton"),
             "clicked", G_CALLBACK(cb_pause), NULL);
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
             "state-set", G_CALLBACK(cb_powerOnOff), NULL);

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
         /* Quit button signal */

         g_signal_connect(
             gtk_builder_get_object(p_builder, "stopButton"),
             "clicked", G_CALLBACK(cb_quit), NULL);

         /* Change screen */

         g_signal_connect(
             gtk_builder_get_object(p_builder, "moveScreen"),
             "clicked", G_CALLBACK(cb_moveScreen), NULL);

         g_signal_connect(
             gtk_builder_get_object(p_builder, "map"),
             "clicked", G_CALLBACK(cb_map), NULL);

         //gtk_widget_set_events(GTK_WIDGET(gtk_builder_get_object(p_builder, "draw1")), GDK_BUTTON_MOTION_MASK | GDK_BUTTON_PRESS_MASK);
         gtk_widget_show_all(p_win);
         switch_block(FALSE);
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
