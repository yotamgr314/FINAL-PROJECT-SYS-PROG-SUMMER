#include "httpd.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define BUF_SIZE 1024

int user_exists(const char* username) {
    FILE *file = fopen("./files/password.txt", "r");
    if (!file) {
        return 0;
    }

    char stored_payload[BUF_SIZE];
    char stored_username[100];
    while (fgets(stored_payload, sizeof(stored_payload), file)) {
        sscanf(stored_payload, "username=%[^&]", stored_username);
        if (strcmp(username, stored_username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int register_user(const char* payload) {
    if (strstr(payload, "username=") == NULL) {
        return 0;
    }

    FILE *file = fopen("./files/password.txt", "a");
    if (!file) {
        return 0;
    }

    fprintf(file, "%s\n", payload);
    fclose(file);
    return 1;
}

int login_user(const char* payload) {
    FILE *file = fopen("./files/password.txt", "r");
    if (!file) {
        return 0;
    }

    char stored_payload[BUF_SIZE];
    while (fgets(stored_payload, sizeof(stored_payload), file)) {
        stored_payload[strcspn(stored_payload, "\n")] = 0;

        if (strcmp(payload, stored_payload) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void send_file(const char *file_path, const char *content_type) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("HTTP/1.1 404 Not Found\r\n\r\n");
        printf("Error: Could not open %s.\r\n", file_path);
        fflush(stdout);
        return;
    }

    char buffer[BUF_SIZE];
    ssize_t bytes_read;

    printf("HTTP/1.1 200 OK\r\n");
    printf("Content-Type: %s\r\n\r\n", content_type);
    fflush(stdout);

    while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
        fflush(stdout);
    }

    close(fd);
}

void route() {
    ROUTE_START()

    // Route for serving the main HTML page
    ROUTE_GET("/")
    {
        send_file("./files/index.html", "text/html");
    }

    // Route for serving the CSS files
    ROUTE_GET("/index_style.css")
    {
        send_file("./files/index_style.css", "text/css");
    }

    ROUTE_GET("/profile_page.css")
    {
        send_file("./files/profile_page.css", "text/css");
    }

    // Route for serving the JS files
    ROUTE_GET("/index_script.js")
    {
        send_file("./files/index_script.js", "application/javascript");
    }

    ROUTE_GET("/profile_page.js")
    {
        send_file("./files/profile_page.js", "application/javascript");
    }

    // Route for serving the lion images
    ROUTE_GET("/lion_sleeping.jpg")
    {
        send_file("./files/lion_sleeping.jpg", "image/jpeg");
    }

    ROUTE_GET("/lion_awake.jpg")
    {
        send_file("./files/lion_awake.jpg", "image/jpeg");
    }

    // Route for serving the profile page
    ROUTE_GET("/profile")
    {
        send_file("./files/profile_page.html", "text/html");
    }

    // Route for serving profile data
    ROUTE_GET("/profileinfo")
    {
        char username[100] = {0};
        char filepath[BUF_SIZE] = {0};
        char profile_data[BUF_SIZE] = {0};

        sscanf(qs, "username=%s", username);

        snprintf(filepath, sizeof(filepath), "./data/%s.data", username);

        FILE *file = fopen(filepath, "r");
        if (!file) {
            printf("HTTP/1.1 404 Not Found\r\n\r\n");
            printf(" ");
            fflush(stdout);
            return;
        }

        fread(profile_data, 1, sizeof(profile_data), file);
        fclose(file);

        printf("HTTP/1.1 200 OK\r\n");
        printf("Content-Type: text/plain\r\n\r\n");
        printf("%s", profile_data);
        fflush(stdout);
    }

    // Route for handling user registration
    ROUTE_POST("/register")
    {
        char username[100] = {0};
        sscanf(payload, "username=%[^&]", username);

        if (user_exists(username)) {
            printf("HTTP/1.1 302 Found\r\n");
            printf("Location: /?response=RegisterFailed\r\n\r\n");
            fflush(stdout);
        } else {
            if (register_user(payload)) {
                printf("HTTP/1.1 302 Found\r\n");
                printf("Location: /?response=RegisterSuccess\r\n\r\n");
                fflush(stdout);
            } else {
                printf("HTTP/1.1 302 Found\r\n");
                printf("Location: /?response=RegisterFailed\r\n\r\n");
                fflush(stdout);
            }
        }
    }

    // Route for handling user login
    ROUTE_POST("/login")
    {
        if (login_user(payload)) {
            char username[100] = {0};
            sscanf(payload, "username=%[^&]", username);
            printf("HTTP/1.1 302 Found\r\n");
            printf("Location: /profile?username=%s\r\n\r\n", username);
            fflush(stdout);
        } else {
            printf("HTTP/1.1 302 Found\r\n");
            printf("Location: /?response=LoginFailed\r\n\r\n");
            fflush(stdout);
        }
    }

    // Route for updating profile information
    ROUTE_POST("/profileinfo")
    {
        char username[100] = {0};
        char profileText[BUF_SIZE] = {0};
        char filepath[BUF_SIZE] = {0};

        sscanf(qs, "username=%s", username);

        strncpy(profileText, payload, sizeof(profileText) - 1);

        snprintf(filepath, sizeof(filepath), "./data/%s.data", username);

        FILE *file = fopen(filepath, "w");
        if (!file) {
            printf("HTTP/1.1 500 Internal Server Error\r\n\r\n");
            printf("Error: Could not open profile file for user '%s'.", username);
            return;
        }

        fprintf(file, "%s", profileText);
        fclose(file);

        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Profile updated successfully for user '%s'.", username);
        fflush(stdout);
    }

    // Add route for logout
    ROUTE_GET("/logout")
    {
        // Assuming no session management is needed, we just redirect to the index page.
        printf("HTTP/1.1 302 Found\r\n");
        printf("Location: /\r\n\r\n");
        fflush(stdout);
    }

    ROUTE_END()
}
