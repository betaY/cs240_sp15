
#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

int top=0;
double stack[MAXSTACK];

void stack_clear() 
{
  top = 0;
}

double stack_pop()
{
	// Add implementation here`
    // int pop = stack[top--];
	return stack[--top];
}

void stack_push(double val)
{
	// Add implementation here
    stack[top++] = val;
}

void stack_print()
{
    printf("Stack:\n");
    if( top == 0) {
        printf("Stack is empty");
    }
    for(int i = 0; i < top; i++) {
        printf("%d: %f\n", i, stack[i]);
    }
}

int stack_top()
{
  return top;
}

int stack_max()
{
  return MAXSTACK;
}

int stack_is_empty()
{
  return top == 0;
}


