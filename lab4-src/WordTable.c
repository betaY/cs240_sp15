
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordTable.h"

// Initializes a word table
void wtable_init(WordTable * wtable)
{
	// Allocate and initialize space for the table
	wtable->nWords = 0;
	wtable->maxWords = 10;
	wtable->wordArray = (WordInfo *) malloc(wtable->maxWords * sizeof(WordInfo));
	for (int i = 0; i < wtable->maxWords; i++) {
		llist_init(&wtable->wordArray[i].positions);
	}
}

// Add word to the tableand position. Position is added to the corresponding linked list.
void wtable_add(WordTable * wtable, char * word, int position)
{
	// Find first word if it exists
	for (int i = 0; i < wtable->nWords; i++) {
		if ( strcmp(wtable->wordArray[i].word, word)== 0 ) {
			// Found word. Add position in the list of positions
			llist_insert_last(&wtable->wordArray[i].positions, position);
			return;
		}
	}
	// Word not found.

	// Make sure that the array has space.
	// Expand the wordArray here.
    if (wtable->nWords < wtable->maxWords) {
        // Add new word and position
        wtable->wordArray[wtable->nWords].word = strdup(word);
        llist_insert_last(&wtable->wordArray[wtable->nWords].positions, position);
        wtable->nWords++;
    } else {
        wtable->maxWords *= 2;
        wtable->wordArray = (WordInfo *) realloc(wtable->wordArray, wtable->maxWords * sizeof(WordInfo));
        for (int i = wtable->nWords; i < wtable->maxWords; i++) {
            llist_init(&wtable->wordArray[i].positions);
        }
        // Add new word and position
        wtable->wordArray[wtable->nWords].word = strdup(word);
        llist_insert_last(&wtable->wordArray[wtable->nWords].positions, position);
        wtable->nWords++;
    }
}

// Print contents of the table.
void wtable_print(WordTable * wtable, FILE * fd)
{
	fprintf(fd, "------- WORD TABLE -------\n");

	// Print words
	for (int i = 0; i < wtable->nWords; i++) {
		fprintf(fd, "%d: %s: ", i, wtable->wordArray[i].word);
		llist_print( &wtable->wordArray[i].positions);
	}
}

// Get positions where the word occurs
LinkedList * wtable_getPositions(WordTable * wtable, char * word)
{
	// Write your code here
    int num = wtable->nWords;
    int i;
    for (i = 0; i < num; i++) {
        if (strcmp(word, wtable->wordArray[i].word) == 0) {
            return &wtable->wordArray[i].positions;
        }
    }
	return NULL;
}

//
// Separates the string into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;
int wordCount;
int charCount;
static int wordPos = 0;

#define isaletter(ch) ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))



// It returns the next word from stdin.
// If there are no more more words it returns NULL.
// A word is a sequence of alphabetical characters.
static char * nextword(FILE * fd) {
	// Write your code here
    int c;
    for (int i = 0; i < 200; i++) {
        word[i] = 0;
    }
    wordLength = 0;
    while ((c=fgetc(fd))!=-1) {
        // c=fgetc(fd);
        if (c == -1)
            return NULL;
        while (c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != -1 && c != '-' && c != '\'') {
//            wordPos++;
            word[wordLength++] = c;
            break;
        }
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '-' || c == '\'') {
            wordPos++;
            if (wordLength != 0) {
//                *pos = wordPos;
                return word;
            }
            else
                continue;
        }
    }
    if (wordLength != 0) {
//        *pos =wordPos;
        return word;
    }
    else
        return NULL;
}

// Conver string to lower case
void toLower(char *s) {
	// Write your code here
	// to convert one char to lower case. You have to do it for all chars in string.
    int i = 0;
    while (s[i] != '\0') {
	    char ch = s[i];
        if (ch>='A'&&ch<='Z') {
            ch = (ch-'A')+'a';
            s[i]=ch;
        }
        if (!isaletter(s[i])) {
            s[i] = ' ';
        }
        i++;
	}    
}

char * trim(char *str) {
    char *out = (char *)malloc(sizeof(str));
    int index=0,i=0;
    while(str[index]==' ') index++;
    for(i=0; i<strlen(str)-index; i++) out[i] = str[i+index];
    out[i]='\0';
    index= strlen(out);
    while(index>0 && out[index-1]==' ')  index--;
    out[index]='\0';
    return out;
}


// Read a file and obtain words and positions of the words and save them in table.
int wtable_createFromFile(WordTable * wtable, char * fileName, int verbose)
{
	// Write your code here
    wtable_init(wtable);
    FILE * fp = fopen(fileName, "r");
    if (!fp) {
        return 0;
    }
    char * word;
    char * cp;
    int length = 0;
    int count = 0;
    while ((word = nextword(fp)) != NULL) {
        toLower(word);
        if ((strlen(word)<=3 && word[0] != ' ') || (strlen(word)>3 && word[2] != ' ')) {
//            length = strlen(word); && word[0] != ' '
            //memcpy(cp, word, sizeof(word));
            //trim(cp);
            if (word[1] == ' ') {
                if (verbose == 1) {
                    printf("%d: word=%s, pos=%d\n", count++, trim(word), wordPos+1);
                }
                wtable_add(wtable, trim(word), wordPos+1);
            } else if (word[0] == ' ') {
                if (verbose == 1) {
                    printf("%d: word=%s, pos=%d\n", count++, trim(word), wordPos);
                }
                wtable_add(wtable, trim(word), wordPos);
            } else {
                if (verbose == 1) {
                    printf("%d: word=%s, pos=%d\n", count++, trim(word), wordPos-1);
                }
                wtable_add(wtable, trim(word), wordPos-1);
            }
        } else if (strlen(word)>=2 && word[1] != ' ') {
            if (word[0] == ' ') {
                if (verbose == 1) {
                    printf("%d: word=%s, pos=%d\n", count++, trim(word), wordPos);
                }
                wtable_add(wtable, trim(word), wordPos);
            }
        }
        
//        printf("%d: word=%s, pos=%d\n", count++, word, wordPos);
        wordPos += strlen(word);
    }
}

// Sort table in alphabetical order.
void wtable_sort(WordTable * wtable)
{
	// Write your code here

    int num = wtable->nWords;
    int i, j;
    // WordInfo * temp = (WordInfo *) malloc(sizeof(WordInfo));
    char * w_temp = (char *)malloc(100);
    LinkedList p_temp;
    llist_init(&p_temp);
    //printf("-------------------------------------\n");
    for (i = 1; i < num; i++)
    {
        for (j = 1; j < num; j++)
        {
            /* code */
            // printf("%s\n", wtable->wordArray[j-1].word);
            if (strcmp(wtable->wordArray[j-1].word, wtable->wordArray[j].word) > 0) {
                strcpy(w_temp,wtable->wordArray[j-1].word);
                strcpy(wtable->wordArray[j-1].word, wtable->wordArray[j].word);
                strcpy(wtable->wordArray[j].word, w_temp);
                memcpy((void *)&p_temp, (void *)&wtable->wordArray[j-1].positions, sizeof(&wtable->wordArray[j-1].positions));
                memcpy((void *)&wtable->wordArray[j-1].positions, (void *)&wtable->wordArray[j].positions, sizeof(&wtable->wordArray[j].positions));
                memcpy((void *)&wtable->wordArray[j].positions, &p_temp, sizeof(&p_temp));
                // memcpy(temp, &wtable->wordArray[j-1], sizeof(&wtable->wordArray[j-1]));
                // memcpy(&wtable->wordArray[j-1], &wtable->wordArray[j], sizeof(&wtable->wordArray[j]));
                // memcpy(&wtable->wordArray[j], temp, sizeof(temp));
            }
        }

    }

}

// Print all segments of text in fileName that contain word.
// at most 200 character. Use fseek to position file pointer.
// Type "man fseek" for more info. 
int wtable_textSegments(WordTable * wtable, char * word, char * fileName)
{
	// Write your code here
    FILE * fp = fopen(fileName, "rb");

    LinkedList * list = wtable_getPositions(wtable, word);
    int num = llist_number_elements(list);
    // llist_print(list);
    ListNode * head = list->head;
    int i;
    printf("===== Segments for word \"%s\" in book \"%s\" =====\n", word, fileName);
    for (i = 0; i < num; i++) {
        printf("---------- pos=%d-----\n", head->value);
        if(fseek(fp, sizeof(char)*head->value, SEEK_SET) != 0) {
            if (ferror(fp)) {
                perror("fseek()");
                fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__-5);
                exit(EXIT_FAILURE);
            }
        }
        printf("......");
        char out[200];
        size_t ret_code = fread(out, sizeof *out, 200, fp);
        if(ret_code == 200) {
            for(int n = 0; n < 200; ++n) printf("%c", out[n]);
            printf("......\n");
        } else { // error handling
            if (feof(fp))
                printf("Error reading: unexpected end of file\n");
            else if (ferror(fp)) {
                perror("Error reading");
            }
        }

        head = head->next;
    }

}

