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

char* vigenere(char* entree, char* ret)
{
	int lg = strlen(entree);
    char k[] = "keyabcd";// only smaller cases
	char m[lg];
	
	strcpy(m, entree);
	//chiffrement vigenere
	int i;// index dans la chaine initiale
	char mc[lg];// message codé
	for (i = 0; i < lg; i++)
	{
		int ch = m[i];
		int key = k[i%strlen(k)] - 97;

		if (97 <= ch && ch <= 122)
		{// lettre minuscule
			mc[i] = (char) (97 + (ch - 97 + key)%26);
		} else if (65 <= ch && ch <= 90)
		{// lettre majuscule
			mc[i] = (char) (65 + (ch - 65 + key)%26);
		} else if (48 <= ch && ch <= 57)
		{// chiffre
			mc[i] = (char) (48 + (ch - 48 + key)%26);
		} else
		{
			mc[i] = m[i];
		}
	}
	mc[lg] = '\0';
	ret = mc;
	return ret;
}

int main(int argc, char *argv [])
{
	
	char *entree = (char*) malloc(1024*sizeof(char));
	char* ip = (char *) malloc(15*sizeof(char));
	
	// parse comand line
	if (argc != 3)
    {
        fprintf(stderr, "Invalid usage. Valid usages is:\nclient -a server_ip\n");
        exit(EXIT_FAILURE);
    }

	// Recuperation des options : str, texte entre; gpio, numero de la gpio
	char* options = "a:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'a':
				if (strlen(optarg) > 15) {
                	printf("The ip address seems to be too long\n");
                	exit(EXIT_FAILURE);
				} else {
                	ip = optarg;
				}
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }   
	
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
	const char *hostname = ip;

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
		fgets(entree, 1024, stdin);
		
		char* cipher = (char*) calloc(strlen(entree), sizeof(char));
		// chiffrement
		cipher = vigenere(entree, cipher);
		char buffer[strlen(entree)];
		strcpy(buffer, cipher);
		printf("\nClear Message:\n%s\n\nEncoded Message:\n%s\n\n", entree, buffer);

		if(send(sock, buffer, strlen(buffer), 0) < 0)
		{	
			perror("send()");
			exit(errno);
		}

		if(buffer[0] == '/' && buffer[1] == 'E' && buffer[2] == 'O' && buffer[3] == 'F')
		{
			signals_handler(SIGINT);
		}
    }

	free(ip);
    exit(EXIT_SUCCESS);
}


