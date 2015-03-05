
#include <stdio.h>

#include "array.h"

// Return sum of the array
double sumArray(int n, double * array) {
  double sum = 0;
  
  double * p = array;
  double * pend = p+n;

  while (p < pend) {
    sum += *p;
    p++;
  }

  return sum;
}

// Return maximum element of array
double maxArray(int n, double * array) {
  int i = 0;
  int max = *array;
  for (i = 0; i < n; i++) {
    if (max < *(array+i))
        max = *(array+i);
  }
  return max;
}

// Return minimum element of array
double minArray(int n, double * array) {
  int i = 0;
  int min = *array;
  for (i = 0; i < n; i++) {
    if (min > *(array+i))
      min = *(array+i);
  }
  return min;
}

// Find the position int he array of the first element x 
// such that min<=x<=max or -1 if no element was found
int findArray(int n, double * array, double min, double max) {
  int i=0;
  for (i=0; i < n; i++) {
    if(*(array+i) > min && *(array+i) < max)
        return i;
  }
  return -1;
}

// Sort array without using [] operator. Use pointers 
// Hint: Use a pointer to the current and another to the next element
int sortArray(int n, double * array) {
    int i, j;
    double tmp;
    for (i = 1; i < n; i++) { 
        tmp = *(array + i);
        for (j = i-1; j >=0 && *(array+j)>tmp; j--) {
            *(array+j+1) = *(array+j);
        }
        *(array+j+1) = tmp;
    }
}

// Print array
void printArray(int n, double * array) {
    int i = 0;
    for (i = 0; i < n; i++) {
        printf("%d:%f\n",i, *(array+i));
    }
}
