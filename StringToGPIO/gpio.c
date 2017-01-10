#include <fcntl.h>
#include "vigenere.h"

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

int main(int argc, char *argv [])
{
	int lg;// longueur de la chaine de caractere
	int fd;// file descriptor
	int gpio;// numero de la gpio
	char buf[255];
	char str[1024];

    // parse comand line
	if (argc != 5)
    {
        fprintf(stderr, "Invalid usage: gpio -n gpio_number -s displayed_string\n");
        exit(EXIT_FAILURE);
    }

	char * options = "n:s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'n':
                gpio = atoi(optarg);
                break;

			case 's':
				strncpy(str, optarg, sizeof str - 1);
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }
	lg = strlen(str);

	// transformation en un tableau de char contenant . ou - (ti ou ta) et separant les mots et lettres
	int i;// index dans la chaine initiale
	char *m = (char *) calloc(6*lg, sizeof(char));// message codé 
	for (i = 0; i < lg; i++)
	{
		int ch = str[i];
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
	printf("\nClear Message: \n%s\n\nMorse Message : \n%s\n\n", str, c);


	// TODO allumer la gpio avec le bon rythme
	// Parametrage de la gpio
	/* 
	gpioExport(gpio); // Rendre la gpio disponible
	gpioDirection (gpio, 1); // gpio en sortie
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio); //mettre dans buf le chemin de la valeur de la gpio
	int fd = open(buf, O_WRONLY); // ouvrir le fichier
	write(fd, buf, 0);// eteindre la gpio
	sleep(5);

	//boucle sur le morse
	int i;
	for (i = 0; i < strlen(c); i++)
	{
		switch (c[i])
		{
			case '.':// ti
    			//write(fd, buf, 1);
				sleep(1);
                break;

			case '-':// ta
    			//write(fd, buf, 1);
				sleep(3);
                break;

            case '/':// separation				
				sleep(2);
				break;

			case ' ':
				break;

			case '?':
                fprintf(stderr, "Invalid character: %c\n", c[i]);
                exit(EXIT_FAILURE);
		}
    	//write(fd, buf, 0);
	}

    //close(fd);//fermeture du fichier
	*/
}

void gpioExport(int gpio)
{
    char buf[255];
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);
}

void gpioDirection(int gpio, int direction) // 1 for output, 0 for input
{
	char buf[255];
    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    int fd = open(buf, O_WRONLY);

    if (direction)
    {
        write(fd, "out", 3);
    }
    else
    {
        write(fd, "in", 2);
    }
    close(fd);
}
