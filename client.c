#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define IP "127.0.0.1"
#define PORT 2222

SOCKET sock;

void signals_handler(int signal_number)
{
    printf("\nSignal catched\n");
    if(send(sock, "/EOF", strlen("/EOF"), 0) < 0)
	{
		perror("send()");
		exit(errno);
	}
	printf("Fermeture des sockets\n");
	close(sock);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv [])
{
	char *entree; 
	entree = malloc(1024);   
	
	// signals handler
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);

    //connexion au serveur
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}
    
	struct hostent *hostinfo = NULL;
	SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */
	const char *hostname = IP;

	hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
	if (hostinfo == NULL) /* l'hôte n'existe pas */
	{
		fprintf (stderr, "Unknown host %s.\n", hostname);
		exit(EXIT_FAILURE);
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin.sin_port = htons(PORT); /* on utilise htons pour le port */
	sin.sin_family = AF_INET;
	
	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");
		exit(errno);
	}


    while(1)
    {
		scanf("%s",entree);
		
		char buffer[1024];
		strcpy(buffer,entree);
		if(send(sock, buffer, strlen(buffer), 0) < 0)
		{
			perror("send()");
			exit(errno);
		}


    }

    exit(EXIT_SUCCESS);
}
