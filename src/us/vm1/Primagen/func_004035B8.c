#include "common.h"
#include "m2c_macros.h"

/* the members are host pointers (Sort_Quick is given their width) */
void MantidQueen_SortSwap(void **arg0, void **arg1) {
    void *t = *arg0;

    *arg0 = *arg1;
    *arg1 = t;
}
