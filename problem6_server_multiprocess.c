#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int read_bytes = read(client_fd, buffer, BUFFER_SIZE);
        if (read_bytes <= 0) {
            printf("Client disconnected.\n");
            close(client_fd);
            exit(0);
        }
        printf("Client: %s\n", buffer);

        // Echo message back to the client
        send(client_fd, buffer, strlen(buffer), 0);
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to a port
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
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }
        printf("New client connected.\n");

        // Create a child process for each client
        if (fork() == 0) {
            // Child process
            close(server_fd); // Close unused server socket in child
            handle_client(client_fd);
        } else {
            // Parent process
            close(client_fd); // Close unused client socket in parent
            waitpid(-1, NULL, WNOHANG); // Clean up zombie processes
        }
    }

    close(server_fd);
    return 0;
}
