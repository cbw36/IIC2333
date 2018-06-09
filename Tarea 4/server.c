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
#include"player.h"
#include <time.h>

//the thread function
char* decCard_tobin(int value);
void *connection_handler(void *);
Player** players;
Card** deck;
int count;
int count_named;


int main(int argc , char *argv[])
{   deck = malloc(52*sizeof(Card));
    players = malloc(2*sizeof(Player));
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    count = 0;
    count_named = 0;

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
        Player* player = malloc(sizeof(Player));
        player->id = *(int*)new_sock;
        players[count] = player;

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
    count ++;
    int sock = *(int*)socket_desc;
    int read_size;
    char *message;
    char * client_message;
    client_message = malloc(20* sizeof(char));

    /** Step 1: get nickname  **/
    char* msg = "0000001000000000";
    write(sock, msg, 96 * sizeof(char));
    puts("sent query");
//    read_size = recv(sock , name_msg_res , 2000 , 0); //TODO how is it time independent lol?
//    printf("Nickname registered as %s", name_msg_res); //TODO set global name

    //TODO add a wait for other player


   while( (read_size = recv(sock , client_message, 20 , 0)) > 0 )//(socket, buffer, length, flags)
   {
       for (int i = 0; i < count; i++) {
         if (players[i]->id == sock){
           players[i]->name = client_message;
           count_named ++;
         }
         printf("nombre:%s, count_named:%i\n", players[i]->name, count_named);
       }
       if (count_named == 2){

           write(players[0]->id, "se a encontado rival: \n" , 20);
           sleep(1);
           write(players[0]->id, players[1]->name, 20);
           write(players[1]->id, "se a encontado rival: \n" , 20);
           sleep(1);
           write(players[1]->id, players[0]->name, 20);
           sleep(1);
           write(players[1]->id, "00000110000000100000001111101000", 80);
           write(players[0]->id, "00000110000000100000001111101000", 80);
           players[0]-> pot = 1000;
           players[1]-> pot = 1000;

       }
   }

   if(read_size == 0)
   {
       puts("Client disconnected");
       fflush(stdout);
   }
   else if(read_size == -1)
   {
       perror("recv failed");
   }

   //Free the socket pointer
   free(socket_desc);

    return 0;
}

void init_deck(){

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 13; i++) {
      Card* card = malloc(sizeof(Card));
      card->value = decCard_tobin(j+1);
      card->used = 0;
      card->shape = decCard_tobin(i+1);
      deck[i*13+j] = card;
    }
  }
}

void resetDeck() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 13; i++) {
      deck[i*13+j]->used = 0;
    }}
}

Card** hand(){
  Card** handPlayer = malloc(5*sizeof(Card));
  int cantidad = 0;
  while (cantidad < 5) {
    int random = rand();
    if (deck[random%52]->used == 0) {
      deck[random%52]->used = 1;
      handPlayer[cantidad] = deck[random%52];
      cantidad ++;
    }

  }
  return handPlayer;
}

char* decCard_tobin(int value){
  if (value == 1) {
    return "00000001";
  }
  else if (value == 2) {
    return "00000010";
  }
  else if (value == 3) {
    return "00000011";
  }
  else if (value == 4) {
    return "00000100";
  }
  else if (value == 5) {
    return "00000101";
  }
  else if (value == 6) {
    return "00000110";
  }
  else if (value == 7) {
    return "00000111";
  }
  else if (value == 8) {
    return "00001000";
  }
  else if (value == 9) {
    return "00001001";
  }
  else if (value == 10) {
    return "00001010";
  }
  else if (value == 11) {
    return "00001011";
  }
  else if (value == 12) {
    return "00001100";
  }
  else if (value == 13) {
    return "00001101";
  }
}
