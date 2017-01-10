#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv [])
{
	int lg = 0;
	char str[1024];
	char k[128];

    // parse comand line
	if (argc != 5)
    {
        fprintf(stderr, "Invalid usage: \nvigenere -k key -s displayed_string\n");
        exit(EXIT_FAILURE);
    }

	char * options = "k:s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
			case 's':
				strncpy(str, optarg, sizeof str -1);
                break;

            case 'k':
				strncpy(k, optarg, sizeof k -1);
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }
	lg = strlen(str);

	//chiffrement cesar
	int i;// index dans la chaine initiale
	char m[lg];// message codé 
	for (i = 0; i < lg; i++)
	{
		int ch = str[i];
		int key = k[i%strlen(k)];
		if (97 <= ch && ch <= 122)
		{// lettre minuscule
			m[i] = (char) (97 + (ch + key)%26);
		} else if (65 <= ch && ch <= 90)
		{// lettre majuscule
			m[i] = (char) (65 + (ch + key)%26);
		} else if (48 <= ch && ch <= 57)
		{// chiffre
			m[i] = (char) (48 + (ch + key)%10);
		} else
		{
			m[i] = str[i];
		}
	}

	printf("\nClear Message: \n%s\n\nKey: \n%s\n\nCoded Message: \n%s\n\n", str, k, m);

	return EXIT_SUCCESS;
}
