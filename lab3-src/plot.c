
#include <stdio.h>
#include <stdlib.h>

#include "rpn.h"

#define MAXCOLS 80
#define MAXROWS 40

char plot[MAXROWS][MAXCOLS];

void clearPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      plot[i][j] = ' ';
    }
  }
}

void printPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      printf("%c", plot[i][j]);
    }
    printf("\n");
  }
}

void plotXY(int x, int y, char c) {
  if ( x <0 || x>=MAXCOLS || y < 0 || y >=MAXROWS) return;
  plot[y][x]=c;
}

void createPlot( char * funcFile, double minX, double maxX) {
  int nvals = MAXCOLS;
  double yy[MAXCOLS];

  clearPlot();

  // Evaluate function and store in vector yy
  for (int i = 0; i < MAXCOLS; i++) {
    double x = ((maxX-minX)/MAXCOLS)*i + minX;
    yy[i] = rpn_eval(funcFile, x);
  }


  //Compute maximum and minimum y in vector yy
  // double maxY = rpn_eval(funcFile, maxX);
  double maxY = yy[0];
  for(int i = 1; i < MAXCOLS; i++){
      if (yy[i] > maxY)
          maxY = yy[i];
  }

  // double minY = rpn_eval(funcFile, minX);
  //double minY = -1 * maxY;
  double minY = yy[0];
  for(int i = 1; i < MAXCOLS; i++) {
      if (yy[i] < minY)
          minY = yy[i];
  }
  //printf("maxY = %f, minY = %f\n", maxY, minY);

  
  //Plot x axis
  for (int i = 0; i < MAXCOLS; i++) {
    if (minY >= 0)
    {
      /* code */
      plotXY(i, (int)MAXROWS-1, '_');
    }
    else
      plotXY(i, ((int)MAXROWS-1)/2, '_');
  }

  //Plot y axis
  for (int i = 0; i < MAXROWS; i++) {
      plotXY(((int)MAXCOLS)/2, i, '|');
  }

  // Plot function. Use scaling.
  // minX is plotted at column 0 and maxX is plotted ar MAXCOLS-1
  // minY is plotted at row 0 and maxY is plotted at MAXROWS-1
  for (int i = 0; i < MAXCOLS; i++) {
      double x = ((maxX-minX)/(MAXCOLS))*i + minX;
      double y = rpn_eval(funcFile, x);
      double unitY = (maxY-minY)/(float)(MAXROWS);
      //printf("%f\t", unitY);
      int tmp = (y-minY)/unitY;
      int y_loc = MAXROWS - 1 - tmp;
      //if((float)(MAXROWS-(y-minY)/unitY)-(float)y_loc<0.5)
        // y_loc++;

      //printf("%d, %f\n", y_loc, y);
      // printf("y = %f,\t minY = %f\t y_loc = %d\n", y, minY, y_loc);
      plotXY(i, y_loc, '*');
      // if (i == 39)
      //    printf("%d\t%f\t%f\t%f\n", y_loc, y, unitY, (y-minY)/unitY);
      //if (i == 40)
      //    printf("%d\t%f\t%f\t%f\n", y_loc, y, unitY, (y-minY)/unitY);

  }

  printPlot();

}

int main(int argc, char ** argv)
{
  printf("RPN Plotter.\n");
  
  if (argc < 4) {
    printf("Usage: plot func-file xmin xmax\n");
    exit(1);
  }

  // Get arguments
  char * funcName = argv[1];
  double xmin = atof(argv[2]);
  double xmax = atof(argv[3]);
  createPlot(funcName, xmin, xmax);
}
