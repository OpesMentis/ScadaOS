#include "vigenere.h"

char* vigenere(char *keyParam, char *strParam, char* ret)
{
	if (strlen(keyParam) >= 128 || strlen(strParam) >= 1024)
	{
		fprintf(stderr, "Invalid usage: at least one of the parameters are too long in vigenere(char*key, char* str)\n");
        exit(EXIT_FAILURE);
	}

	int lg = 0;
	char str[1024];
	char k[128];

	// cast to char[]
	strncpy(k, keyParam, sizeof k -1);
	strncpy(str, strParam, sizeof str -1);
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
	ret = m;

	return ret;
}
