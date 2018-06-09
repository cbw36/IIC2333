/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include"helperFunctions.c"
#include"player.h"
#include <time.h>
#include<unistd.h>

char*getName(char* resp_id);
int * messageToBytes(int message);
char* strToBin(char* str);


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
    server.sin_port = htons( 8080 );

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
        char* name_query;
        name_query = malloc(sizeof(char)*4000);

        if( recv(sock , name_query , 4000, 0) > 0)
        {
            char* id = malloc(8);
            if (strlen(name_query)>8) {
              for (int i = 0; i < 8; i++) {
                id[i] = name_query[i];
              }
            }

            if (strncmp(id, "00000011", 8) == 0)
            {
              char* name;
              char* resp_id = "00000100";
              name = getName(resp_id);
              if( send(sock , name , strlen(name) , 0) < 0)
              {
                puts("Send failed");
                return 1;
              }
            }
            if (strncmp(id, "00000110", 8) == 0)
            {
                puts("Se te han otorgado 1000 dolares");
            }
            if (strncmp(id, "00001000", 8) == 0)
            {
                puts("Se ha iniciado correctamente la partida");
            }

            if (strncmp(id, "00001001", 8) == 0)
            {
                puts("Se a realizado la apuesta inicial de 10 dolares");
            }
            if (strncmp(id, "00001010", 8) == 0)
            {
                int* decode = binToInt(name_query);
                for (int i = 1; i < 5; i++) {
                  printf("Card%i value:%i shape:%i\n", i ,decode[2*i],decode[2*i+1]);
                }
                puts("Se a entregado las 5 cartas");
                sleep(1);
                if( send(sock , "00001100" , 17 , 1) < 0)
                {
                  puts("Send failed");
                  return 1;
                }
            }
            //int * msg_digits = malloc(20);
            //msg_digits = messageToBytes(name_query);
            //printf("%i, %i, %i\n", msg_digits[0], msg_digits[1],msg_digits[2]);

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

char* getName(char* resp_id)
{
    char* name = malloc(20);
    printf("Please enter your name to be used while playing: ");
    scanf("%s" , name);
    int len = strlen(name);

    char* name_bin = malloc(8*len);
    name_bin = strToBin(name);

    int* msg_size_int = malloc(8*sizeof(int));
    char* msg_size = malloc(8);
    msg_size_int = dec_to_bin(strlen(name), msg_size_int, 7);
    for (int i = 0;i<8;i++)
      msg_size[i] = msg_size_int[i] + '0';


  char* name_msg = malloc(16 + strlen(name_bin));
  for (int i = 0; i<8;i++)
    name_msg[i] = resp_id[i];
  for (int i = 8; i<16;i++)
    name_msg[i]= msg_size[i-8];
  for (int i = 16;i<16+ strlen(name_bin);i++)
    name_msg[i] = name_bin[i-16];
  puts(name_msg);

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
