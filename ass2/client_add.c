/*
This program first checks the number of command-line arguments provided by the user, and if there are less than three (i.e., the program name and two integers), it prompts the user to enter the integers at runtime. Otherwise, it converts the command-line arguments to integers using atoi().

Next, it creates a socket and connects to the server using the socket(), gethostbyname(), connect() functions. The server address is hardcoded to localhost with a port of 12345.

Then, it formats the two integers as a string using sprintf() and sends them to the server using the write() function. It then reads the server's response into the buffer using the read() function and converts the response to an integer using atoi().

Finally, it displays the sum of the two integers to the user and closes the socket using close().
*/
#include <stdio.h> //standard input/output functions such as printf()
#include <stdlib.h> // functions such as memory allocation and random number generation.
#include <string.h> //functions for manipulating strings such as strlen(), strcpy(), and strcat().
#include <unistd.h> //contains various constants and function declarations for POSIX (Portable Operating System Interface) operating systems.
#include <sys/types.h> //contains various data types used by the operating system, such as pid_t and size_t.
#include <sys/socket.h> //provides the definitions of socket-related structures, such as struct sockaddr and the socket() function for creating a socket.
#include <netinet/in.h> //includes the definitions of Internet Protocol (IP) networking structures, such as struct sockaddr_in for storing IP addresses and port numbers.
#include <netdb.h> 

#define PORT 12345

int main(int argc, char *argv[]) {
    int sockfd, n; //socket file descripto and  n store the number of bytes read from or written to the socket.
    struct sockaddr_in serv_addr; //information about the server address.
    struct hostent *server;

    char buffer[256];
    int num1, num2, result;

    if (argc < 3) {
        printf("Enter the first number: ");
        scanf("%d", &num1);
        printf("Enter the second number: ");
        scanf("%d", &num2);
    } else {
        num1 = atoi(argv[1]);
        num2 = atoi(argv[2]);
    }

    //create a new socket and returns a file descriptor for that socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"Error: no such host\n");
        exit(1);
    }

    //sizeof - sets the bytes of memory
    memset((char *) &serv_addr, 0, sizeof(serv_addr)); 
    //specifies that the IP addresses used will be in the IPv4 format.
    serv_addr.sin_family = AF_INET;
    memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);
    //set the port number of the server that the client wants to connect to
    //hton() convert the port number from host byte order to network byte order, which is the byte order used by the network.  
    serv_addr.sin_port = htons(PORT);

    //connect to remote server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    sprintf(buffer, "%d %d", num1, num2);
    //send data over a socket.
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    //reading data from a socket file descriptor sockfd and storing it in a buffer.
    memset(buffer, 0, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer)-1);
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    result = atoi(buffer);
    printf("The sum of %d and %d is %d\n", num1, num2, result);

    close(sockfd);
    return 0;
}
