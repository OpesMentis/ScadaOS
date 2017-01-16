#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv [])
{
	int fd;// file descriptor
	int gpio;// numero de la gpio
	char buf[255];

    // parse comand line
	if (argc != 3)
    {
        fprintf(stderr, "Invalid usage. Valid usages is:\ntest -n gpio_number\n");
        exit(EXIT_FAILURE);
    }

	// Recuperation des options : str, texte entre; gpio, numero de la gpio
	char* options = "n:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'n':
                gpio = atoi(optarg);
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }
	printf("Blinking gpio number %d\n", gpio);

	// TODO allumer la gpio avec le bon rythme
	// Parametrage de la gpio
	
	gpioExport(gpio); // Rendre la gpio disponible
	gpioDirection (gpio, 1); // gpio en sortie
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio); //mettre dans buf le chemin de la valeur de la gpio
	fd = open(buf, O_WRONLY); // ouvrir le fichier
	write(fd, buf, 0);// eteindre la gpio
	sleep(5);

	//boucle sur le morse
    int i = 10;
	while (i>0) {
		write(fd, buf, 1);
		sleep(1);
		write(fd, buf, 0);
		i--;
	}

    close(fd);//fermeture du fichier

	return EXIT_SUCCESS;
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
