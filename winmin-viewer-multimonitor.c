//WIP for winmin-viewer multimonitor support
#include <gtk/gtk.h>
#include <spice-client-gtk-3.0/spice-client-gtk.h>
#include <stdio.h>
#include <glib-object.h>
#include <spice-client.h>
#include <signal.h> 

SpiceSession * session;
char** name;
char** sock;
int id;
int windowcount;
GtkWidget * window[2];
SpiceChannel * mainchannel;

gboolean resize_done (gpointer data)
{
  guint *gid = data;
  *gid = 0;

  resize();
  
  return FALSE;
}


gboolean on_configure_event (GdkEvent *event, gpointer data)
{
  static guint gid = 0;
  if (gid)
    g_source_remove (gid);
  gid = g_timeout_add (250, resize_done, &gid);
  return FALSE;
  
}

void resize() {
  printf("Resize!\n");

  for (int i = 0; i < windowcount; ++i) {
    int width, height;
    gtk_window_get_size(window[i], &width, &height);
    spice_main_set_display_enabled(mainchannel,i,TRUE);
    spice_main_set_display(mainchannel,i, 0, 0, width, height);

  }
  //spice_main_channel_update_display(mainchannel, 1, 0, 0, 500, 500, TRUE);
  printf("Send!\n");
  spice_main_send_monitor_config(mainchannel);
  //spice_main_set_display_enabled(mainchannel,-1,TRUE);


}


void setup_spice() {
  session = spice_session_new();
  g_object_set(session,"uri",sock);
  printf("test\n");
  char** test;
  g_object_get(session,"uri",&test);
  printf("test: %s\n",test);

  mainchannel =  spice_channel_new(session,1,0);
  spice_session_connect(session);
  if (SPICE_IS_MAIN_CHANNEL(mainchannel)) {
    printf("Good\n");
  }
  else {
    printf("Bad\n");
  }
}


static void
activate (GtkApplication* app,
          gpointer        user_data)
{

  window[windowcount] = gtk_application_window_new (app);

  SpiceDisplay * display = spice_display_new(session, windowcount);

  GtkWidget * displaywidget = GTK_WIDGET(display);
  gtk_container_add(GTK_CONTAINER(window[windowcount]),displaywidget);
    
  gtk_window_set_title (GTK_WINDOW(window[windowcount]), name);
  gtk_window_set_default_size (GTK_WINDOW (window[windowcount]), 900, 800);
  gtk_window_set_position(GTK_WINDOW(window[windowcount]), GTK_WIN_POS_CENTER);
  gtk_widget_show_all (window[windowcount]);

  resize();
  g_signal_connect(G_OBJECT(window[windowcount]), "configure-event",
        G_CALLBACK(on_configure_event), NULL);
  windowcount += 1;
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;
  name = argv[1];
  sock = argv[2];
  id = (int)strtol(argv[3], NULL, 10);
  app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
  
  

  setup_spice();
  windowcount = 0;
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  status = g_application_run (G_APPLICATION (app), NULL, NULL);

  g_object_unref (app);

  return status;
}




/*static void
activateaux (GtkApplication* app,
          gpointer        user_data)
{

  window[1] = gtk_application_window_new (app);

  SpiceDisplay * display2 = spice_display_new(session,1);

  GtkWidget * displaywidget = GTK_WIDGET(display2);
  gtk_container_add(GTK_CONTAINER(window[1]),displaywidget);
    
  gtk_window_set_title (GTK_WINDOW(window[1]), name);
  gtk_window_set_default_size (GTK_WINDOW (window[1]), 900, 800);
  gtk_window_set_position(GTK_WINDOW(window[1]), GTK_WIN_POS_CENTER);
  gtk_widget_show_all (window[1]);

  resize();
  g_signal_connect(G_OBJECT(window[1]), "configure-event",
        G_CALLBACK(on_configure_event), NULL);
}
*/
