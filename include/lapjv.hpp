#ifndef __LAPJV_HPP
#define __LAPJV_HPP

typedef int row;
typedef int col;
template <typename cost>
int lap(int dim,
        cost **assigncost,
        col *rowsol,
        row *colsol,
        cost *u,
        cost *v,
        const cost nonassignmentcost);

template <typename cost>
void checklap(int dim, cost **assigncost,
              col *rowsol, row *colsol, cost *u, cost *v);
#endif
