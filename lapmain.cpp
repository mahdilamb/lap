/************************************************************************
*
*  lapmain.cpp
   version 1.0 - 4 September 1996
   author: Roy Jonker @ MagicLogic Optimization Inc.

   main program file to run and check Jonker-Volgenant LAP code
*
*************************************************************************/

#include "lapjv.hpp"
#include <random>
#include <stdio.h>

#define COSTRANGE 1000.0
#define PRINTCOST 1
typedef int cost;
int main()
{

  int dim, startdim = 4, enddim = 4;
  cost **assigncost, *u, *v, lapcost;
  int i, *colsol;
  int j, *rowsol;


  printf("\ndimensions %d .. %d\n", startdim, enddim);

  assigncost = new cost *[enddim];
  for (i = 0; i < enddim; i++)
    assigncost[i] = new cost[enddim];

  rowsol = new int[enddim];
  colsol = new int[enddim];
  u = new cost[enddim];
  v = new cost[enddim];
std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, COSTRANGE); 
  for (dim = startdim; dim <= enddim; dim++)
  {
   
    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
        assigncost[i][j] = (cost)(distr(gen));

#if (PRINTCOST)
    for (i = 0; i < dim; i++)
    {
      printf("\n");
      for (j = 0; j < dim; j++)
        printf("%4d ", assigncost[i][j]);
    }
#endif
  
    lapcost = lap<int>(dim, assigncost, rowsol, colsol, u, v, 100000);
    printf("\n\ndim  %4d - lap cost %5d\n", dim, lapcost);

    checklap<int>(dim, assigncost, rowsol, colsol, u, v);
  }

  delete[] assigncost;
  delete[] rowsol;
  delete[] colsol;
  delete[] u;
  delete[] v;

  return 0;
}
