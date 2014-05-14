#include <iostream>
#include <gtk/gtk.h>
using namespace std;

GtkWidget* window;
GtkWidget* entry;
GtkWidget* button;
GtkWidget* hbox;

void button_clicked (GtkWidget* button, gpointer data) {
	const char* cmd = gtk_entry_get_text (GTK_ENTRY ((GtkWidget *) data));
	execlp (cmd, cmd, NULL);
}
int main (int argc, char **argv) {
	gtk_init (&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);

	entry = gtk_entry_new ();
	button = gtk_button_new_with_label ("OK");
	g_signal_connect (GTK_OBJECT (entry), "activate", GTK_SIGNAL_FUNC (button_clicked), entry);
	g_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (button_clicked), entry);
	hbox = gtk_vbox_new (true, 5);
	GtkWidget* label = gtk_label_new ("Please input command!");
	gtk_box_pack_start (GTK_BOX (hbox), label, true, true, 0);
	gtk_box_pack_start (GTK_BOX (hbox), entry, true, true, 0);
	gtk_box_pack_start (GTK_BOX (hbox), button, true, false, 0);

	gtk_container_add (GTK_CONTAINER (window), hbox);
	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}
