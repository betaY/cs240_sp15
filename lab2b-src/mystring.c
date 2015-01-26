
#include <stdlib.h>
#include "mystring.h"

// Type "man string" to see what every function expects.

int mystrlen(char * s) {
    int count = 0;

    while(*s++ != '\0')
        count++;
	return count;
}

char * mystrcpy(char * dest, char * src) {
    char *temp = dest;

    while ((*dest++ = *src++) != '\0');
    

	return temp;
}

char * mystrcat(char * dest, char * src) {
    char *temp = dest;
    while(*dest++ != '\0');
    while((*dest++ = *src++) != '\0');
    *dest = '\0';
	return temp;
}

int mystrcmp(char * s1, char * s2) {
	return -1;
}

char * mystrstr(char * hay, char * needle) {
	return NULL;
}

char * mystrdup(char * s) {
	return NULL;
}

char * mymemcpy(char * dest, char * src, int n)
{
	return NULL;
}

