/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include"helperFunctions.c"

char*getName();
int * messageToBytes(int message);


int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000];
    char *server_reply = malloc(2000 * sizeof(char));
    char*name = malloc(16 * sizeof(char));

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        int name_query;

        if( read(sock , &name_query , 2000) > 0)
        {
            printf("%i, %x\n", name_query, name_query);
            int * msg_digits = malloc(20);
            msg_digits = messageToBytes(name_query);
            printf("%i, %i, %i\n", msg_digits[0], msg_digits[1],msg_digits[2]);
////            execMsgReq(msg_digits[0]);
//            printf("Server request: %s \n", server_reply);
//            server_reply = malloc(2000 * sizeof(char));
        }

//        printf("Enter response to request: ");
//        scanf("%s" , message);

        //Send some data
//        if( send(sock , message , strlen(message) , 0) < 0)
//        {
//            puts("Send failed");
//            return 1;
//        }

//        //Receive a reply from the server
//        if( recv(sock , server_reply , 2000 , 0) < 0)
//        {
//            puts("recv failed");
//            break;
//        }

//        printf("Server reply: %s \n", server_reply);
//        server_reply = malloc(2000 * sizeof(char));
//        puts(server_reply);

    }

    close(sock);
    return 0;
}

char* getName()
{
    char* name = malloc(20);
    printf("Please enter your name to be used while playing: ");
    scanf("%s" , name);
    return name;
}

int * messageToBytes(int message)
{
    int * digits = malloc(20 * sizeof(int));
    int loc = 0;
    while(message != 0) {
        int digit = message % 16;
        message = message / 16;
        digits[loc] = digit;
        loc++;
    }
    return digits;
}

//execMsgReq(int msg_)