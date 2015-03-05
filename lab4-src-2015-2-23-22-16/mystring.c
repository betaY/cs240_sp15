
#include <stdlib.h>
#include <string.h>
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
    if((dest==NULL)||(src==NULL))
        return NULL;
    char *temp = dest;
    while(*dest != '\0')
        dest++;
    while(*src != '\0')
        *dest++ = *src++;
    *dest = '\0';
	return temp;
}

int mystrcmp(char * s1, char * s2) {
    //while((s1!=NULL) && (s2!=NULL))
    while(*s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    if (*s1-*s2 > 0)
        return 1;
    else if (*s1-*s2 == 0)
        return 0;
    else 
        return -1;
}

char * mystrstr(char * hay, char * needle) {
    int i;
    if (*needle) {
        while(*hay) {
            for (i = 0; *(hay+i) == *(needle+i); i++) {
                if (!*(needle+i+1)) {
                    return (char *)hay;
                }
            }
            hay++;
        }
        return NULL;
    }
	return (char *)hay;
}

char * mystrdup(char * s) {
    char *temp;
    if(s != NULL) {
        temp = (char *)malloc(mystrlen(s)+1);
        strcpy(temp, s);
        return temp;
    }
    return NULL;
}

char * mymemcpy(char * dest, char * src, int n)
{
    if ((dest == NULL) || (src == NULL))
        return NULL;
    
    unsigned char* src_copy = (char *)src;
    unsigned char* dest_copy = (char *)dest;
    while (n-- > 0)
        *dest_copy++ = *src_copy++;

	return dest;
}

