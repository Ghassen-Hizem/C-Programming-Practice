//
// Created by Hizem on 4/15/24.
// Exercice 2 TP1 programmation réseaux
// Exemple de programme Client qui posséde les utilités suivantes :
// - résolution d'adresse IP en utilisant le HostName
// - Création de socket et connection a un serveur web en utilisant TCP et protocol IPV4
// - envoyer une requete HTTP 1.1 au serveur web
// - recevoir la réponse de GET HTTP REQUEST de la part de serveur
// - Affichage de la réponse récu
//librarie standard des fonctions usuelles
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define MYMSGLEN 2000

int main(int argc, char *argv[]) {
    /*
     * Initialisation des variables utilies
     * sock : variable qui contient la socket de connection Client
     * server : une structure qui contient tout les informations sur le serveur dont on va communiquer avec
     * message : message de client (HTTP request) qu'on va envoyer
     * server_reply : HTTP response du serveur
     * he: pointeur qui pointe a l'adresse IP de serveur après résolution d'adresse
     * inaddr : adresse IP de serveur qui a été résolue en utilsant le hostName*/
    int sock;
    struct sockaddr_in server;
    char *message, server_reply[MYMSGLEN];
    struct hostent *he;
    //adresse IP resolu du serveur
    struct in_addr inaddr;

    /*
     * Création du socket a envoyer au serveur web
     * */

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("could not create the socket");
        return -1;
    }

    printf("socket created");

//    Trouver l'adresse IP de site web avec la fonction gethostbyname()

    he = gethostbyname("httpd.apache.org");
//vérification du bon fonctionnement de la méthode et du serveur
    if (he == NULL) {
        perror("gethostbyname");
        return 2;
    }

    //faire une copie de l'adresse he->h_addr vers la variable inaddr
    memcpy((char *) &inaddr, he->h_addr, sizeof(struct in_addr));

    //inet_ntoa : fonction qui fait la translation de l'adresse IP vers ascii pour étre lisible
    printf("IP adress for httpd.apache.org is %s \n", inet_ntoa(inaddr));
//Remplissage d'informations sur le serveur pour établir la communication
    server.sin_addr.s_addr = inaddr.s_addr;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    //connection au serveur

    int res;
    res = connect(sock, (struct sockaddr *) &server, sizeof(server));
//vérifier que la connection fonctionne
    if (res < 0) {
        printf("connection error");
        return 1;
    }

    printf("connected\n");
    //requete HTTP a envoyer
    message = "GET / HTTP/1.1\r\nhost: httpd.apache.org \r\n\r\n";
//envoie de la requete HTTP
    if (send(sock, message, strlen(message), 0) < 0) {
        printf("send failed\n");
        close(sock);
        return -1;
    }

    puts ( "message request sent" ) ;

    /*
     * memset() : sets the n bytes of server_reply to 0 , utilisé pour intialisation de la variable server_reply
     * n : MYMSGLEN
     * */
    memset(server_reply, 0, MYMSGLEN);
    int reponse;

//    Reception de la réponse du serveur
//recv : recoit la réponse de serveur vers le socket
//sock : socket utilisé pour la connection
//server_reply : variable pour stocker la réponse
// MYMSGLEN: taille maximale de la chaine server_reply
//0 flags
    while ((reponse = recv(sock, server_reply, MYMSGLEN, 0)) != 0) {

        if (reponse < 0) {
            puts("recv failed");
            break;
        } else {
            //affichage de la réponse du serveur
            printf("%s", server_reply);
            //initialisation de la variable server_reply de nouveau
            memset(server_reply, 0, MYMSGLEN);
        }
    }
    //fermeture de la connection après avoir recu la réponse
    close(sock);
    return 0;
    
}



