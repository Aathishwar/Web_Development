// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int sockfd;
char username[50];

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("[-] Disconnected from chat.\n");
            exit(0);
        }
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    pthread_t recv_thread;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        exit(1);
    }

    // Get username
    printf("Enter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // remove newline

    // Send username to server
    send(sockfd, username, strlen(username), 0);

    printf("Connected to chat room at 127.0.0.1:8080 as [%s]\n", username);

    // Create thread for receiving messages
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // Message sending loop
    char message[BUFFER_SIZE];
    while (1) {
        fgets(message, sizeof(message), stdin);
        if (strlen(message) > 1) {
            send(sockfd, message, strlen(message), 0);
        }
    }

    close(sockfd);
    return 0;
}
