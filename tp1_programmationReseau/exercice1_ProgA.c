//
// Created by hizem on 4/15/24.
// Programme Client standard
//
//Library standard de C pour les fonctions usuelles
#include <stdio.h>
//library String pour utilisation de chaines de caractéres dans C
#include <string.h>
//library de Socket Library pour la communication réseau
#include <sys/socket.h>
//utils pour la library socket
#include <arpa/inet.h>
#include <unistd.h>
#include <semaphore.h>
#define MYMSGLEN 2000

int main( int argc, char *argv[])
{

    int sock;
    struct sockaddr_in server;
    char message[MYMSGLEN], server_reply[MYMSGLEN];

    /*
     *
     * Création d'une socket coté client pour intialisation de la communication
     * utilisation du protocol IPV4
     */


    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock==-1) {
        printf("could not create socket\n");
        return -1;
    }

    printf("socket created \n");

    /*
     * spécification de l'addresse IP , famille de l'adresse et port de serveur
     * avec lequel on doit communiquer
     */

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);


    /*
     * Établissement de connection avec le serveur en utilisant la fonction connect()
     * si res <0 , la connection a échoué et ne peut pas étre établie
     * sinon , la connection était établie et on attend l'acceptation du serveur
     */

    int res;
    if((res = connect(sock , (struct sockaddr *) &server , sizeof(server))) <0) {
        perror("connection failed error\n");
        close(sock);
        return -1;
    }

    printf("Returned Value %d\n",res);
    printf("connection established, waiting to be accepted...");


    /*
     * Saisie de message a envoyé au serveur
     */

    while(1) {
        memset( message , 0 , MYMSGLEN);
        printf("\nPLease type a message to transfer for processing:");
        scanf("%s",message);


    /*
     * si il'ya une erreur d'envoie dans le message , on supprime la connection établie
     * et on retourne -1 a la fonction main
     * on a envoyé le message au serveur avec la fonction send()
     */

    if(send(sock,message,strlen(message),0)<0) {
        printf("send failed\n");
        close(sock);
        return -1;
    }


    /*
     * recevoir la réponse de serveur et affichage de l'output
     * vérification de la bonne réception de message sinon levée d'une exception
     */

    memset(server_reply,0,MYMSGLEN);
    if(recv(sock,server_reply, MYMSGLEN , 0)<0) {
        printf("recv failed\n");
        close(sock);
        return -1;
    }

    printf("server reply:%s",server_reply);
    }


}

