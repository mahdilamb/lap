#ifndef __LAP_HPP
#define __LAP_HPP


/************************************************************************
*
*  lap.cpp
   version 1.0 - 4 September 1996
   author: Roy Jonker @ MagicLogic Optimization Inc.
   e-mail: roy_jonker@magiclogic.com

   Code for Linear Assignment Problem, according to

   "A Shortest Augmenting Path Algorithm for Dense and Sparse Linear
    Assignment Problems," Computing 38, 325-340, 1987

   by

   R. Jonker and A. Volgenant, University of Amsterdam.
*
*************************************************************************/
template <typename T>
int lapjv(int dim,
          T **assigncost,
          int *rowsol,
          int *colsol,
          T *u,
          T *v);
/**
	Min cost bipartite matching via shortest augmenting paths

	This is an O(n^3) implementation of a shortest augmenting path
	algorithm for finding min cost perfect matchings in dense
	graphs.  In practice, it solves 1000x1000 problems in around 1
	second.
  
  	cost[i][j] = cost for pairing left node i with right node j
  	Lmate[i] = index of right node that left node i pairs with
  	Rmate[j] = index of left node that right node j pairs with
	The values in cost[i][j] may be positive or negative.  To perform
	maximization, simply negate the cost[][] matrix.

	Taken from https://github.com/jaehyunp/
	Adapted by https://github.com/cheind
*/
template <typename T>
int lapsap(int n, T **cost, int *rowsol, int *colsol, T *u,
            T *v);

template <typename T>
void checklap(int dim, T **assigncost,
              int *rowsol, int *colsol, T *u, T *v);
#endif
