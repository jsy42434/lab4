#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_client(int client_fd);
void handle_get(int client_fd, const char *path);
void handle_post(int client_fd, const char *path, const char *body);
void send_response(int client_fd, const char *status, const char *content_type, const char *body);

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Webserver listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle the client in the same process (for simplicity)
        handle_client(client_fd);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE], method[16], path[256], protocol[16];
    memset(buffer, 0, BUFFER_SIZE);

    // Read HTTP request
    read(client_fd, buffer, BUFFER_SIZE - 1);
    sscanf(buffer, "%s %s %s", method, path, protocol);

    // Check HTTP method
    if (strcmp(method, "GET") == 0) {
        handle_get(client_fd, path);
    } else if (strcmp(method, "POST") == 0) {
        // Find body of the POST request
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4; // Skip the "\r\n\r\n"
            handle_post(client_fd, path, body);
        } else {
            send_response(client_fd, "400 Bad Request", "text/plain", "Missing POST body");
        }
    } else {
        send_response(client_fd, "405 Method Not Allowed", "text/plain", "Only GET and POST are supported");
    }
}

void handle_get(int client_fd, const char *path) {
    if (strcmp(path, "/") == 0) {
        send_response(client_fd, "200 OK", "text/html", "<h1>Welcome to the Webserver</h1>");
    } else {
        send_response(client_fd, "404 Not Found", "text/plain", "File not found");
    }
}

void handle_post(int client_fd, const char *path, const char *body) {
    if (strcmp(path, "/cgi-bin/echo") == 0) {
        char response_body[BUFFER_SIZE];
        snprintf(response_body, BUFFER_SIZE, "CGI Program Response: %s", body);
        send_response(client_fd, "200 OK", "text/plain", response_body);
    } else {
        send_response(client_fd, "404 Not Found", "text/plain", "CGI program not found");
    }
}

void send_response(int client_fd, const char *status, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n"
             "%s",
             status, content_type, strlen(body), body);
    send(client_fd, response, strlen(response), 0);
}
