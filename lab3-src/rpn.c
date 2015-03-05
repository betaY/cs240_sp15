
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include "rpn.h"
#include "nextword.h"
#include "stack.h"

double rpn_eval(char * fileName, double x) {

    double a, b, c;
    double val;
    double out;
    FILE *fd;

    fd = fopen(fileName, "r");
    char * w;
    while ( (w = nextword(fd)) != NULL) {
 //       printf("%s\n", w);

        if (strcmp(w,"+") == 0) {
            val = stack_pop();
            val += stack_pop();
            stack_push(val);
        } else if (strcmp(w,"-") == 0) {
            val = stack_pop();
            val = stack_pop() - val;
            stack_push(val);
        } else if (strcmp(w,"*") == 0) {
            val = stack_pop();
            val *= stack_pop();
            stack_push(val);
        } else if (strcmp(w,"/") == 0) {
            val = stack_pop();
            val = stack_pop() / val;
            stack_push(val);
        } else if (strcmp(w,"sin") == 0) {
            val = stack_pop();
            val = sin(val);
            stack_push(val);
        } else if (strcmp(w,"cos") == 0) {
            val = stack_pop();
            val = cos(val);
            stack_push(val);
        } else if (strcmp(w,"pow") == 0) {
            val = stack_pop();
            val = pow(stack_pop(), val);
            stack_push(val);
        } else if (strcmp(w,"exp") == 0) {
            val = stack_pop();
            val = exp(val);
            stack_push(val);
        } else if (strcmp(w,"log") == 0) {
            val = stack_pop();
            val = log(val);
            stack_push(val);
        } else if (strcmp(w,"x") == 0) {
            stack_push(x);
        } else {
            val =  atof(w);
            stack_push(val);
        }
    }
    if (stack_top() == 1)
        return stack_pop();
    else if (stack_top() == 0) {
        printf("Stack underflow\n");
        exit(-1);
    } else {
        printf("Elements remain in the stack\n");
        exit(-1);
    }
}

