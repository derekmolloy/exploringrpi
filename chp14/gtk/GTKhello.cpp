#include<gtk/gtk.h>

int main(int argc, char *argv[]){
   // This application will have a window and a single label
   GtkWidget *window, *label;
   // Initialize the toolkit, pass the command line arguments
   gtk_init(&argc, &argv);
   // Create the top-level window (not yet visible)
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   // Set the title of the window to Exploring RPi
   gtk_window_set_title ( GTK_WINDOW (window), "Exploring RPi");
   // Create a label
   label = gtk_label_new ("Hello Raspberry Pi");
   // Add the label to the window
   gtk_container_add(GTK_CONTAINER (window), label);
   // Make the label visible (must be done for every widget)
   gtk_widget_show(label);
   // Make the window visible
   gtk_widget_show(window);
   // Runs the main loop until gtk_main_quit() is called  (hit Ctrl C)
   gtk_main();
   return 0;
}
