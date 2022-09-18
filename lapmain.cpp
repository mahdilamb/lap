/************************************************************************
*
*  lapmain.cpp
   version 1.0 - 4 September 1996
   author: Roy Jonker @ MagicLogic Optimization Inc.

   main program file to run and check Jonker-Volgenant LAP code
*
*************************************************************************/

#include "system.h"
#include "lap.cpp"

#define COSTRANGE 1000.0
#define PRINTCOST 1
typedef int cost;
int main()
{

  int dim, startdim, enddim;
  cost **assigncost, *u, *v, lapcost;
  int i, *colsol;
  int j, *rowsol;
  double runtime;

  printf("start dimension ?\n");
  scanf("%d", &startdim);
  printf("end dimension ?\n");
  scanf("%d", &enddim);
  printf("\ndimensions %d .. %d\n", startdim, enddim);

  assigncost = new cost *[enddim];
  for (i = 0; i < enddim; i++)
    assigncost[i] = new cost[enddim];

  rowsol = new int[enddim];
  colsol = new int[enddim];
  u = new cost[enddim];
  v = new cost[enddim];

  for (dim = startdim; dim <= enddim; dim++)
  {
    seedRandom(1000 * dim);
    /* in Visual C++ the first random numbers are not very random.
       call random couple of times before we really start. */
    random();
    random();
    random();
    random();
    random();
    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
        assigncost[i][j] = (cost)(random() * (double)COSTRANGE);

#if (PRINTCOST)
    for (i = 0; i < dim; i++)
    {
      printf("\n");
      for (j = 0; j < dim; j++)
        printf("%4d ", assigncost[i][j]);
    }
#endif

    printf("\nstart\n");
    runtime = seconds();
    lapcost = lap<int>(dim, assigncost, rowsol, colsol, u, v, 100000);
    runtime = seconds() - runtime;
    printf("\n\ndim  %4d - lap cost %5d - runtime %6.3f\n", dim, lapcost, runtime);

    checklap<int>(dim, assigncost, rowsol, colsol, u, v);
  }

  delete[] assigncost;
  delete[] rowsol;
  delete[] colsol;
  delete[] u;
  delete[] v;

  printf("\n\npress key\n");
  char c;
  scanf("%d", ((int *)&c));
  return 0;
}
