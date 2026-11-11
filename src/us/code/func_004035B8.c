#include "common.h"
#include "m2c_macros.h"

#ifdef TARGET_PORT
/* the members are host pointers (Sort_Quick is given their width) */
void MantidQueen_SortSwap(void **arg0, void **arg1) {
    void *t = *arg0;

    *arg0 = *arg1;
    *arg1 = t;
}
#else
void MantidQueen_SortSwap(s32 *arg0, s32 *arg1) {
    s32 temp_v1;

    temp_v1 = *arg0;
    *arg0 = *arg1;
    *arg1 = temp_v1;
}
#endif
