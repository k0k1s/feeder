#include <gtk/gtk.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>

#define BUFFER_SIZE 8192

// Callback function to handle curl data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    size_t current_size = strlen((char *)userp);
    if (current_size + total_size < BUFFER_SIZE - 1) {
        strncat((char *)userp, (char *)contents, total_size);
        return total_size;
    }
    return 0;
}

// Function to fetch news from the backend
void fetch_news(char *buffer, size_t buffer_size) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/news");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            snprintf(buffer, buffer_size, "curl_easy_perform() failed: %s", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    } else {
        snprintf(buffer, buffer_size, "Failed to initialize curl.");
    }
}

// Function to create a GTK window with the news content
void on_activate(GtkApplication *app) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *textview;
    GtkWidget *status_label;
    GtkTextBuffer *buffer;
    char news_buffer[BUFFER_SIZE] = {0};

    // Fetch news and display status
    fetch_news(news_buffer, BUFFER_SIZE);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "News Feeder");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a text view to display the news
    textview = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), textview, TRUE, TRUE, 0);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(buffer, news_buffer, -1);

    // Create a status label to show connection status
    status_label = gtk_label_new(news_buffer);
    gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.newsfeeder", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
