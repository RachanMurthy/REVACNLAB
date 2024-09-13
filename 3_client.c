#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>          // For open()
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>      // For inet_addr()

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[4096], *serv_ip;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s server_ip filename port\n", argv[0]);
        exit(0);
    }

    serv_ip = argv[1];
    portno = atoi(argv[3]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in creating socket");
        exit(1);
    }

    printf("Client online.\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error in connection setup");
        exit(1);
    }

    write(sockfd, argv[2], strlen(argv[2]) + 1);

    bzero(buffer, 4096);
    n = read(sockfd, buffer, 4096);
    if (n <= 0) {
        perror("File not found");
        exit(0);
    }

    write(1, buffer, n); // Write to stdout
    close(sockfd);

    return 0;
}
