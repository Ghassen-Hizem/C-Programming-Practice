//
// Created by hizem on 4/15/24.
// Programme Serveur standard
//


//Library standard de C pour les fonctions usuelles
#include <stdio.h>
#include <stdlib.h>
//library String pour utilisation de chaines de caractéres dans C
#include <string.h>
//library de Socket Library pour la communication réseau
#include <sys/socket.h>
//utils pour la library socket
#include <arpa/inet.h>
#include <unistd.h>


#define MYMSGLEN 2000

int main(int argc,char *argv[]) {

    int socket_desc,client_sock;
    int socket_size,read_size;
    struct sockaddr_in server,client;
    char client_message[MYMSGLEN];

    /*
     * Création de socket serveur pour communication avec le client
     */

    socket_desc = socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc == -1) {
        perror("could not create socket");
        return -1;
    }

    printf("socket created\n");


    /*
     *Setting the options of the socket
     * on a remplacé l'option so_reuseADDR par la valeur de enable = 1
     */

    int code,enable = 1;

    code = setsockopt(socket_desc,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
    if(code ==-1) {
        perror("setsockopt");
        return 1;
    }

    /*
     * famille de communciation = IPV4
     * addresse ip client = ANY
     * PORT CLIENT = 8888
     */

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    /*
     * faire le bind entre le socket client et le socket serveur
     */

    if(bind(socket_desc,(struct sockaddr *) &server,sizeof(server)) <0) {
        //print error message
        perror("bind failed. Error");
        close(socket_desc);
        return -1;
    }

    printf("bind done\n");


    /*
     * tranformer le socket en une socket passive , qui recoit les requetes client
     * activation de listen state
     */

    code = listen(socket_desc,3);

    if(code == -1) {
        perror("listen");
        exit(1);
    }

    /*
     *Accepter la connection du client , et lever d'exception au cas de non établisement
     */

    printf("Waiting for incoming connections...\n");

    socket_size = sizeof(struct sockaddr_in);

    client_sock = accept(socket_desc, (struct  sockaddr *) &client , (socklen_t *) &socket_size);

    if(client_sock <0)  {
        close(socket_desc);
        perror("accept failed");
        return -1;
    }

    printf("Connection accepted \n");


    /*
     * affichage de message recu ainsi que sa taille
     */

    while((read_size = recv(client_sock, client_message,MYMSGLEN,0)) >0) {
        client_message[read_size] = '\0';
        printf("Msg received: %s, size of the message received,%d\n",client_message,read_size);


        /*
         * retourner le meme message au client spécifié
         */

        write(client_sock,client_message,read_size);


    }

    if(read_size ==0) {
        printf("client disconnected\n");
        fflush(stdout);
    }

    else if(read_size == -1) {
        printf("recv failed");
    }

    close(socket_desc);
    close(client_sock);

    return 0;


}


