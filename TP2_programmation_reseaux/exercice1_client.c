//
// Created by hizem on 4/27/24.
// Programme SERVEUR qui vérifie si une chaine est palindrome ou non
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MYSYSGEN 2000


int main(int argc,char *argv[]){

    /*
     * Initialisation de :
     * - socket client
     * - structure l'adresse de serveur
     * - message de client a envoyer
     * - Message de réponse de serveur
     * */
    int sock;
    struct sockaddr_in server;
    char message[MYSYSGEN],server_reply[MYSYSGEN];

    //création de socket IPV4 + TCP + 0 pour le protocol par defaut

    sock = socket(AF_INET,SOCK_STREAM,0);
//erreur checking
    if(sock==-1) {
        perror("error creating socket");
        return -1;
    }

    printf("socket created successfully\n");
/*
 * Spécification de l'adresse + port + protocol family du serveur
 *
 * */
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8888);
    server.sin_family = AF_INET;

    //établissement de connection avec le serveur
    /*
     * Arguments :
     * - sock : socket created de client
     * - (struct sockaddr *) &server : adresse du variable server qui contient les informations du serveur
     * - sizeof(server) : taille de la variable server
     * */

    int res;
    if((res= connect(sock,(struct sockaddr *) &server,sizeof(server))) <0) {
        perror("connection not established correctly");
        close(sock);
        return -1;
    }

    printf("connection established,waiting to be accepted\n");

    //Saisie de message a envoyé
    /*
     * Utilisation de la fonction memset() pour initialisation d'une chaine de caractére au chaine vide
     * */
    memset(message,0,MYSYSGEN);
    /*
     * strcmp(ch1,ch2) : returns 0 if ch1 == ch2 (compare les deux chaines)
     *
     * */
    while(strcmp(message,"#quit") !=0) {
        //initialisation de chaine message
        memset(message,0,MYSYSGEN);
        printf("please type the message you want to send\n");
        scanf("%s",message);

        //send message to server
        /*
         * Arguments :
         * - sock : socket de client
         * - message : message a envoyé
         * - strlen(message) : longeur de message
         * - flags : 0 pour no flags
         * */

        if(send(sock,message, strlen(message),0) <0) {
            printf("send failed");
            close(sock);
            return -1;
        }

        //recevoir la réponse du serveur et affichage de l'output

        memset(server_reply,0,MYSYSGEN);

        if(recv(sock,server_reply, MYSYSGEN,0)<0) {
            printf("message not received correctly\n");
            close(sock);
            return -1;

        }

        printf("server reply%s\n",server_reply);

    }

    return 0;
}