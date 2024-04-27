//
// Created by hizem on 4/26/24.
//

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYMSGLEN 2000

int palindrome ( char * s )
{
    char * t = s + strlen ( s ) - 1 ;

    while ( * s == * t )
        s ++, t -- ;
    return s >= t ;
}

int main(int argc,char *argv[]){

    int sock_serveur,sock_client;
    char message[MYMSGLEN];
    struct sockaddr_in server,client;
    int socket_size,read_size;


    /*
     * Création du socket serveur pour la connection TCP
     * */

    sock_serveur = socket(AF_INET,SOCK_STREAM,0);
    if(sock_serveur == -1 ) {
        perror("socket creation failed");
        return -1;
    }

    printf("socket created !\n");

    /*
     * Setting the reuse address socket option to true
     * 1ér paramétre 'fd' : le file descriptor de socket server
     * 2éme parametre 'level' : SQL_SOCKET = protocol level 1 (application layer)
     * 3éme parametre 'SO_REUSEADDR' : le nom d'option qu'on veut changer
     * 4éme parametre 'optval' : valeur a mettre dans l'option (ici c'est enable = 1 )
     * 5éme 'optlen' : size of optval value
     * */

    int code,enable = 1;

    code = setsockopt(sock_serveur,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
    if(code ==-1) {
        perror("error in setsockopt");
        return -1;
    }

    /*
     * Configuration des caractéristiques pour faire un bind du socket serveur
     * on doit spécifier le IP , port et famille du protocol utilisé
     * */

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    /*
    * faire le bind entre le socket client et le socket serveur
     * params :
     *       fd : file descriptor (int)
     *       addr : (struct sockaddr *) &server avec server est les caractéristiques définies du bind
     *       len : sizeof(server)
     *
    */


    int b = bind(sock_serveur,(struct sockaddr *) &server, sizeof(server));

    if(b<0) {
        perror("bind failed");
        close(sock_serveur);
        return -1;
    }

    printf("bind done\n");

    /*rendre la socket passive en executant la fonction Listen
     *
     *
     * */
while(1) {


    code = listen(sock_serveur,3);
    if(code == -1) {
        perror("error");
        exit(1);
    }

    printf("waiting for incoming connections");

    //size de la structure qui définie les caractéristiques de la socket
    socket_size = sizeof(struct sockaddr_in);

    //acceptation d'une connection client :

     sock_client = accept(sock_serveur,(struct sockaddr *) &client , (socklen_t *) &socket_size);


     if(sock_client <0) {
         close(sock_serveur);
         perror("acceptation ereur");
         return -1;
     }

    printf("connection established\n");
    socklen_t len;
    len = sizeof ( struct sockaddr_in );
    struct sockaddr_in peerAddr ;
    struct sockaddr_in newAddr ;
    //getsockname : returns local address in the variable newAddr
     int code1= getsockname(sock_serveur,(struct sockaddr *) &newAddr , &len);
     //getpeername : returns peer address in the variable peerAddr
    int code2 = getpeername ( sock_client, ( struct sockaddr * ) & peerAddr, & len );
     char reponse1[] = "palindrome";
     char reponse2[] = "paspalindrome";


    printf ( " The local address bound to the new socket --> %s:%d \n" , inet_ntoa ( newAddr.sin_addr ), ntohs ( newAddr.sin_port )  ) ;

    printf ( " The peer address bound to the new socket --> %s:%d \n" , inet_ntoa ( peerAddr.sin_addr ), ntohs ( peerAddr.sin_port )  ) ;

     while((read_size = recv(sock_client,message,MYMSGLEN,0)) >0) {
         message[read_size] = '\0';

         //envoyer la réponse au serveur

         if(palindrome(message)) {
             write(sock_client,reponse1, sizeof(reponse1));
         }
         else {
             write(sock_client,reponse2, sizeof(reponse2));
         }




     }

     if(read_size ==0) {
         printf("client disconnected");
     }
     else if (read_size == -1) {
         printf("recv failed");
     }

    close(sock_client);

}
    return 0;

}

