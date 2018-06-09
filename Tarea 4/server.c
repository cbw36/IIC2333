/*
    C socket server example, handles multiple clients using threads
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0); //(domain, communication type, protocol
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; //IP address
    server.sin_port = htons( 8888 ); // IP port

    //Bind socket to address and port number specified above
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) //(sockfd, sockaddr, addrlen)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3); // 3 is max # of connections to server in queue

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ) //connect to first request in queue
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message;
    char * client_message;
    client_message = malloc(2000* sizeof(char));

    /** Step 1: get nickname  **/
    int name_msg_que = 0xF00;
    write(sock, &name_msg_que, sizeof(name_msg_que)* sizeof(int));
    puts("sent query");
//    read_size = recv(sock , name_msg_res , 2000 , 0); //TODO how is it time independent lol?
//    printf("Nickname registered as %s", name_msg_res); //TODO set global name

    //TODO add a wait for other player




    //Receive a message from client
//    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )//(socket, buffer, length, flags)
//    {
//        //Send the message back to client
//        write(sock , client_message , strlen(client_message)); //(output file, buffer, length)
//        client_message = malloc(2000* sizeof(char));
//    }
//
//    if(read_size == 0)
//    {
//        puts("Client disconnected");
//        fflush(stdout);
//    }
//    else if(read_size == -1)
//    {
//        perror("recv failed");
//    }
//
//    //Free the socket pointer
//    free(socket_desc);

    return 0;
}