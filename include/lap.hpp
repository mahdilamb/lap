#ifndef __LAP_HPP
#define __LAP_HPP

typedef int row;
typedef int col;
template <typename cost>
int lapjv(int dim,
        cost **assigncost,
        col *rowsol,
        row *colsol,
        cost *u,
        cost *v);



template <typename cost>
void checklap(int dim, cost **assigncost,
              col *rowsol, row *colsol, cost *u, cost *v);
#endif
