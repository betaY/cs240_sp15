
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on
    int i, j;
    int count = 0;
    int add = 0;
    add = 16 - (len%16);
    if (add%16 == 0)
        add =0;
    if (len % 16 != 0)
        len += add;

    for (i = 0; i < len; i++) {
        if (i % 16 == 0)
            fprintf(fd, "0x%016lX: ", (unsigned long) p+i);
        if (i < len - add) {
            fprintf(fd, "%02X ", *(p+i)&0xFF);
            count++;
        }
        else 
            fprintf(fd, "%02s ", " ");
        //if (len % 16 != 0) {

        //}

        if (i % 16 == 15) {
            fprintf(fd, " ");
            for (j = 15; j > 15-count; j--) {
                if((*(p+i-j)>=32)&&(*(p+i-j)<=127))
                    fprintf(fd, "%c",*(p+i-j));
                else
                    fprintf(fd, ".");
            }
            fprintf(fd, "\n");
            count = 0;
        }
    }

    //fprintf(fd, "0x%016lX: ", (unsigned long) p);
    int c = p[0]&0xFF;

    // Print first byte as hexadecimal
    //fprintf(fd, "%02X ", c);

    // Print first byte as character
    //fprintf(fd, "%c", (c>=32)?c:'.');
    //fprintf(fd,"\n");
}

