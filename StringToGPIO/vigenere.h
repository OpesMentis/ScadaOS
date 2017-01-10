#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char* vigenere(char *keyParam, char *strParam, char* ret);
// keyParam is max 128 char, strParam and ret is max 1024 char
