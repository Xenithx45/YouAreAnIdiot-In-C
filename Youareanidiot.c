#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define NUM_WINDOWS 30

// Structure to hold window data
typedef struct {
    GtkWidget *window;
    gint x, y;
    gint dx, dy;
    gint width, height;
} PopupWindow;

PopupWindow popups[NUM_WINDOWS];

// Screen dimensions
gint screen_width, screen_height;

// Get the screen size
void get_screen_size(GtkWidget *window) {
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_screen(display, 0);
    screen_width = gdk_screen_get_width(screen);
    screen_height = gdk_screen_get_height(screen);
}

// Function to update the window positions and make them bounce
gboolean move_windows(GtkWidget *widget, GdkEvent *event, gpointer data) {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        PopupWindow *popup = &popups[i];
        
        // Move the window
        popup->x += popup->dx;
        popup->y += popup->dy;
        
        // Check if the window hits the edge and reverse direction if necessary
        if (popup->x <= 0 || popup->x + popup->width >= screen_width) {
            popup->dx = -popup->dx;
        }
        if (popup->y <= 0 || popup->y + popup->height >= screen_height) {
            popup->dy = -popup->dy;
        }

        // Set the new position for the window
        gtk_window_move(GTK_WINDOW(popup->window), popup->x, popup->y);
    }
    
    return TRUE;
}

// Function to create a new popup window
void create_popup(int index) {
    PopupWindow *popup = &popups[index];
    
    // Create a new window
    popup->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(popup->window), "YouAreAnIdiot");
    gtk_window_set_default_size(GTK_WINDOW(popup->window), 300, 350);
    gtk_widget_set_size_request(popup->window, 100, 100);
    
    // Get window dimensions
    gtk_window_get_size(GTK_WINDOW(popup->window), &popup->width, &popup->height);
    
    // Initialize the position and movement direction
    popup->x = rand() % (screen_width - 100); // Random initial position
    popup->y = rand() % (screen_height - 100);
    popup->dx = rand() % 3 + 2;  // Random speed for movement in the x-direction
    popup->dy = rand() % 3 + 2;  // Random speed for movement in the y-direction
    
    // Connect signals
    g_signal_connect(popup->window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    // Show the window
    gtk_widget_show_all(popup->window);
}

int main(int argc, char *argv[]) {
    GtkWidget *main_window;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a main window (to keep GTK event loop running)
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Bouncing Popups");
    g_signal_connect(main_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    
    // Get the screen size
    get_screen_size(main_window);

    // Create multiple popups
    srand(time(NULL));  // Seed the random number generator
    for (int i = 0; i < NUM_WINDOWS; i++) {
        create_popup(i);
    }

    // Set a timeout to move the windows every 10 milliseconds
    g_timeout_add(10, (GSourceFunc)move_windows, NULL);

    // Start GTK main loop
    gtk_main();

    return 0;
}