#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 12345

//error handling
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd, newsockfd, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    //opens a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    //set all bytes of serv_addr to 0 to ensure that any unused fields are set to a safe value.
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    //bind the socket sockfd to the address specified by serv_addr
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Error on binding");
    }

    listen(sockfd, 5);
    printf("Server is listening on port %d...\n", PORT);

    //a while loop to ensure continues listening of the server
    while (1) {
        clilen = sizeof(cli_addr);
        //blocks the program until a client connection is received.
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("Error on accept");
        }

        //create a new child process
        pid = fork();
        if (pid < 0) {
            error("Error on fork");
        }

        if (pid == 0) {  // child process
            close(sockfd);

            char buffer[256];
            int num1, num2, sum;

            memset(buffer, 0, sizeof(buffer));
            int n = read(newsockfd, buffer, sizeof(buffer)-1);
            if (n < 0) {
                error("Error reading from socket");
            }

            sscanf(buffer, "%d %d", &num1, &num2);
            sum = num1 + num2;

            sprintf(buffer, "%d", sum);
            n = write(newsockfd, buffer, strlen(buffer));
            if (n < 0) {
                error("Error writing to socket");
            }

            close(newsockfd);
            exit(0);
        } else {  // parent process
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}
