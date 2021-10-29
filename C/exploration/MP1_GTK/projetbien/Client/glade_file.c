#include <stdlib.h>
#include "stdbool.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include "client.h"

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

static void cb_ok(GtkWidget *p_wid, gpointer p_data)
{
   GtkWidget *p_dialog = NULL;
   const char *p_text = p_data;

   if (p_text != NULL)
   {
      p_dialog = gtk_message_dialog_new(
          NULL,
          GTK_DIALOG_MODAL,
          GTK_MESSAGE_INFO,
          GTK_BUTTONS_OK,
          p_text);

      gtk_dialog_run(GTK_DIALOG(p_dialog));
      gtk_widget_destroy(p_dialog);
   }
}
static void switch_block(bool state)
{
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "pauseButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "buttonPlay")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "leftButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "rightButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "forwardButton")), state);
   gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "movebackButton")), state);
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
   client_run_write(LEFT);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_right(GtkWidget *p_wid, gpointer p_data)
{
   client_run_write(RIGHT);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_forward(GtkWidget *p_wid, gpointer p_data)
{
   client_run_write(FORWARD);
}

/**
 * @brief define the remote action and allow the car to 
 * 
 * @param p_wid 
 * @param p_data 
 */

static void cb_moveback(GtkWidget *p_wid, gpointer p_data)
{
   client_run_write(MOVEBACK);
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
      client_stop();
      switch_block(FALSE);
   }
   else
   {
      client_start();
      gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(p_builder, "buttonPlay")), TRUE);
   }
}

int main(int argc, char **argv)
{

   GError *p_err = NULL;

   /* Initialisation de GTK+ */
   gtk_init(&argc, &argv);
   client_new();

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
             "clicked", G_CALLBACK(cb_left), NULL);

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

         /* Stop button signal */

         g_signal_connect(
             gtk_builder_get_object(p_builder, "stopButton"),
             "clicked", G_CALLBACK(cb_quit), NULL);
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
