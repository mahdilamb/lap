/************************************************************************
*
*  lapmain.cpp
   version 1.0 - 4 September 1996
   author: Roy Jonker @ MagicLogic Optimization Inc.

   main program file to run and check Jonker-Volgenant LAP code
*
*************************************************************************/

#include "lapjv.hpp"
#include "utils.cpp"
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <string>

typedef int cost;

int main(int argc, char *argv[])
{
  int cost_max = 1000;
  bool hide_matrix = false;
  int dim = 4;

  ArgConsumer arg_consumer{"LAP.JV usage"};
  arg_consumer
      .add_argument(
          "--dim",
          [&dim](char *new_dim)
          { dim = stoi(std::string(new_dim)); },
          "(int) Dimension of the matrix")
      .add_argument(
          "--hide-matrix",
          [&hide_matrix](char *ignored)
          { hide_matrix = true; },
          "If present will not print the cost matrix", true)
      .add_argument(
          "--cost-max",
          [&cost_max](char *new_cost_range)
          { cost_max = stoi(std::string(new_cost_range)); },
          "The upper limit of the random numbers used for the cost matrix")
      .consume(argc, argv);

  cost **assigncost, *u, *v, lapcost;
  int i, *colsol;
  int j, *rowsol;

  assigncost = new cost *[dim];
  for (i = 0; i < dim; i++)
    assigncost[i] = new cost[dim];
  fill_random<cost>(assigncost, dim, cost_max);
  rowsol = new int[dim];
  colsol = new int[dim];
  u = new cost[dim];
  v = new cost[dim];

  lapcost = lap<int>(dim, assigncost, rowsol, colsol, u, v, cost_max + 1);
  checklap<int>(dim, assigncost, rowsol, colsol, u, v);

  if (!hide_matrix)
  {
    std::set<std::tuple<int, int>> idx;
    for (int i = 0; i < dim; ++i)
    {
      idx.insert({i, colsol[i]});
    }
    std::cout << "Cost matrix: " << std::endl;
    highlighted_print<cost>(const_cast<const cost **>(assigncost), dim, idx);
    std::cout << std::endl;
  }
  std::cout << "Assignments: " << std::endl;
  std::cout << " col -> row" << std::endl;

  for (int i = 0; i < dim; ++i)
  {
    std::cout << std::setw(4) << i << " -> " << colsol[i] << std::endl;
  }
  delete[] assigncost;
  delete[] rowsol;
  delete[] colsol;
  delete[] u;
  delete[] v;

  return 0;
}