#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>  // For bzero()

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int fd, sockfd, newsockfd, clilen, portno, n;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[4096];

    if (argc < 2) {
        fprintf(stderr, "Error: No port provided\n");
        exit(1);
    }

    portno = atoi(argv[1]);  // Convert the port number from string to integer

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // Create a TCP socket
    if (sockfd < 0)
        error("Error opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));  // Clear the server address structure

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Accept connections from any IP address
    serv_addr.sin_port = htons(portno);  // Set the port number

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Error on binding");

    listen(sockfd, 5);  // Listen for incoming connections, with a backlog of 5
    clilen = sizeof(cli_addr);
    printf("Server waiting for client request\n");

    while (1) {
        // Accept a connection from a client
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("Error on accept");

        bzero(buffer, 4096);
        n = read(newsockfd, buffer, 4096);  // Read the filename from the client
        if (n < 0)
            error("Error reading from socket");

        fd = open(buffer, O_RDONLY);  // Open the requested file
        if (fd < 0)
            error("Error opening file");

        // Read the file and send its contents to the client
        while ((n = read(fd, buffer, 4096)) > 0) {
            if (write(newsockfd, buffer, n) < 0)
                error("Error writing to socket");
        }

        printf("File transfer completed\n");
        close(fd);        // Close the file descriptor
        close(newsockfd); // Close the client socket
    }
    close(sockfd);  // Close the server socket (unreachable code in this case)
    return 0;
}
