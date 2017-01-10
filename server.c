#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define IP "127.0.0.1"
#define PORT 2222

SOCKET sock;
SOCKET csock;

static char* MORSE[128] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, "-.-.--", ".-..-.", NULL, "...-..-", NULL, ".-...", ".----.",
        "-.--.", "-.--.-", NULL, ".-.-.", "--..--", "-....-", ".-.-.-", "-..-.",
        "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
        "---..", "----.", "---...", "-.-.-.", NULL, "-...-", NULL, "..--..",
        ".--.-.", ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
        ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--",
        "-..-", "-.--", "--..", NULL, NULL, NULL, NULL, "..--.-",
        NULL, ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
        ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--",
        "-..-", "-.--", "--..", NULL, NULL, NULL, NULL, NULL,
};

void signals_handler(int signal_number)
{
    printf("\nSignal catched\n");
	printf("Fermeture des sockets\n");
	close(sock);
	close(csock);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv [])
{

	int lg;// longueur de la chaine de caractere
	int fd;// file descriptor
	int gpio=1;// numero de la gpio
	char buf[255];
	char str2[1024];
	char* str = (char*) calloc(1024, sizeof(char));

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
    
  	SOCKADDR_IN sin = { 0 };

	sin.sin_addr.s_addr = htonl(INADDR_ANY); 

	sin.sin_family = AF_INET;

	sin.sin_port = htons(PORT);

	if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
	{
	    perror("bind()");
	    exit(errno);
	}

	if(listen(sock, 5) == SOCKET_ERROR)
	{
		perror("listen()");
		exit(errno);
	}

	SOCKADDR_IN csin = { 0 };
	

	socklen_t sinsize = sizeof csin;

	csock = accept(sock, (SOCKADDR *)&csin, &sinsize);

	if(csock == INVALID_SOCKET)
	{
		perror("accept()");
		exit(errno);
	}

    while(1)
    {
		

		char buffer[1024];
		int n = 0;

		

		if((n = recv(csock, buffer, sizeof buffer - 1, 0)) < 0)
		{
			perror("recv()");
			exit(errno);
		}

		buffer[n] = '\0';

		printf("%s\n",buffer);

		// copiage du pointeur en tableau pour faciliter le parcours
		lg = strlen(buffer);
		strncpy(str2, buffer, sizeof str2 - 1);
		printf("\nClear Message: \n%s\n\n", str2);

		// transformation en un tableau de char contenant . ou - (ti ou ta) et separant les mots et lettres
		int i;// index dans la chaine initiale
		char *m = (char *) calloc(6*lg, sizeof(char));// message codé 
		for (i = 0; i < lg; i++)
		{
			int ch = str2[i];
			if (ch != 32)
			{//pas un espace
				char *code = MORSE[ch];
				strcat(m, code);
			}

		if (i != (lg - 1))
		{
			strcat(m, "/");
		}
	}

	char c[strlen(m) + 1];// tableau de char contenant . ou -
	strncpy(c, m, sizeof c);
	printf("Morse Message : \n%s\n\n", c);

		if(buffer[0] == '/' && buffer[1] == 'E' && buffer[2] == 'O' &&buffer[3] == 'F'){
			signals_handler(SIGINT);
		}
    }

    exit(EXIT_SUCCESS);
}
