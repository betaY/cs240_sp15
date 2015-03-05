
#include <stdio.h>
#include <stdlib.h>

//
// Separates the file into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;

// It returns the next word from fd.
// If there are no more more words it returns NULL. 
char * nextword(FILE * fd) {
  	int c;
    for (int i = 0; i < 200; i++) {
        word[i] = 0;
    }
    wordLength = 0;
    while ((c=fgetc(fd))!=-1) {
    // c=fgetc(fd);
        if (c == -1)
            return NULL;
        while (c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != -1) {
            word[wordLength++] = c;
            break;
        }
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            if (wordLength != 0)
                return word;
            else
                continue;
        }
    }
    //return word;
	// While it is not EOF read char
		// While it is not EOF and it is a non-space char
		// store character in word.
		// if char is not in word return word so far.
		//
	// Return null if there are no more words
    if (wordLength != 0)
        return word;
    else
        return NULL;

}

