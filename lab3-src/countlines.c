
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char ** argv) {
	int c;
	int lines=0;

 	// Add your implementation here
    printf("Program that counts lines.\n");
    char * fileName = argv[1];
    FILE * fd = fopen(fileName, "r");
    if (fd == NULL) {
        printf("Could not open files %s\n", fileName);
        exit(1);
    }
    while ((c = fgetc(fd)) != -1) {
        if (c == '\n')
            lines++;
    }
    
    printf("Total lines: %d\n", lines);

	exit(0);
}
