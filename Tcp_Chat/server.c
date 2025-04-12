// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
char usernames[MAX_CLIENTS][50];

void broadcast_message(const char *message, int sender_fd) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] != 0 && clients[i] != sender_fd) {
            send(clients[i], message, strlen(message), 0);
        }
    }
}

int main() {
    int server_fd, new_socket, max_sd, sd, activity;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    socklen_t addrlen = sizeof(address);

    // Initialize client sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i] = 0;
    }

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    address.sin_port = htons(8080);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Chat server started on port 8080...\n");

    while (1) {
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0) {
                perror("Accept");
                exit(EXIT_FAILURE);
            }

            // Receive username
            char username[50];
            memset(username, 0, sizeof(username));
            recv(new_socket, username, sizeof(username), 0);

            printf("[+] New connection: %s\n", username);

            // Add to list of clients
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i] == 0) {
                    clients[i] = new_socket;
                    strncpy(usernames[i], username, sizeof(usernames[i]) - 1);
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i];

            if (FD_ISSET(sd, &readfds)) {
                int valread = recv(sd, buffer, BUFFER_SIZE, 0);
                if (valread == 0) {
                    printf("[-] %s disconnected\n", usernames[i]);
                    close(sd);
                    clients[i] = 0;
                    usernames[i][0] = '\0';
                } else {
                    buffer[valread] = '\0';

                    char message[BUFFER_SIZE + 64];
                    snprintf(message, sizeof(message), "[%s] %s", usernames[i], buffer);
                    printf("%s", message);
                    broadcast_message(message, sd);
                }
            }
        }
    }

    return 0;
}
