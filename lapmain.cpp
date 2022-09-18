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
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <set>

#include <functional>

typedef int cost;
typedef std::map<std::string, std::function<void(char *)>> argmap;
typedef std::set<std::string> argflags;

/// @brief Parse the args based on the provided function map and flags
/// @param argc the number of arguments
/// @param argv the array of char* representing the args
/// @param args the map of the arg to the function
/// @param flags a set representing functions that toggle on features
void parse_args(int argc, char *argv[], argmap &args, std::set<std::string> &flags)
{
  std::function<void(char *)> fn = nullptr;
  for (int argi = 0; argi < argc; ++argi)
  {
    if (fn != nullptr)
    {
      fn(argv[argi - 0]);
      fn = nullptr;
    }
    if (args.contains(argv[argi]))
    {
      fn = args[argv[argi]];
      if (flags.contains(argv[argi]))
      {
        fn(nullptr);
        fn = nullptr;
      }
    }
  }
  if (fn != nullptr)
  {
    fn(nullptr);
    fn = nullptr;
  }
}

void fill_with_random(cost **assigncost, int dim, int cost_range)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, cost_range);

  for (int i = 0; i < dim; i++)
    for (int j = 0; j < dim; j++)
      assigncost[i][j] = (cost)(distr(gen));
}

void array_printer(const cost **assigncost, int dim)
{
  std::cout << "[";
  for (int i = 0; i < dim; i++)
  {
    if (i)
    {
      std::cout << " ";
    }

    std::cout << "[";
    for (int j = 0; j < dim; j++)
      std::cout << std::setw(4) << assigncost[i][j] << (j != dim - 1 ? "," : "")
                << " ";
    std::cout << "]" << (i != dim - 1 ? ",\n" : "");
  }
  std::cout << "]" << std::endl;
}

void cost_printer(const cost **assigncost, int dim)
{
  int i, j;
  std ::cout << "    ";
  for (j = 0; j < dim; j++)
    std::cout << std::setw(4) << j << " ";

  for (i = 0; i < dim; i++)
  {
    std::cout << std::endl
              << std::setw(4) << i;
    for (j = 0; j < dim; j++)
      std::cout << std::setw(4) << assigncost[i][j];
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  int cost_range = 1000;
  bool print_cost = true;
  bool print_array = false;
  int dim = 4;
  std::set<std::string> flags{"--print-array", "--print-matrix", "--help"};
  std::map<std::string, std::string> arg_descriptions
  {
    {
        "--dim", "(int) Dimension of the matrix"},
        {"--print-matrix", "If present will print the cost matrix"},
        {"--print-array", "If present will print the cost matrix in a form that makes it easier to copy into other languages (e.g. python)"},
    {
      "--cost-range", "The upper limit of the random numbers used for the cost matrix"
    }
  };
  argmap args{
      {"--dim", [&dim](char *new_dim)
       {
         dim = stoi(std::string(new_dim));
       }},
      {"--help", [arg_descriptions](char *value)
       {
         std::cout << "LAP.JV usage" << std::endl;

         for (auto it = arg_descriptions.begin(); it != arg_descriptions.end(); it++)
         {
           std::cout << std::setw(30) << it->first
                     << " : " << it->second
                     << std::endl;
         }
         std::exit(0);
       }},
      {"--print-array", [&print_array](char *ignored)
       {
         print_array = true;
       }},
      {"--print-matrix", [&print_cost](char *ignored)
       {
         print_cost = true;
       }},
      {"--cost-range", [&cost_range](char *new_cost_range)
       {
         cost_range = stoi(std::string(new_cost_range));
       }}};

  parse_args(argc, argv, args, flags);
  cost **assigncost, *u, *v, lapcost;
  int i, *colsol;
  int j, *rowsol;

  assigncost = new cost *[dim];
  for (i = 0; i < dim; i++)
    assigncost[i] = new cost[dim];

  rowsol = new int[dim];
  colsol = new int[dim];
  u = new cost[dim];
  v = new cost[dim];
  fill_with_random(assigncost, dim, cost_range);

  if (print_cost)
  {
    cost_printer(const_cast<const cost **>(assigncost), dim);
  }
  if (print_array)
  {
    array_printer(const_cast<const cost **>(assigncost), dim);
  }

  lapcost = lap<int>(dim, assigncost, rowsol, colsol, u, v, 100000);

  checklap<int>(dim, assigncost, rowsol, colsol, u, v);

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
