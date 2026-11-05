#include "common.h"
#include "structs/world.h"

/* Curve i: the table starts with the element size, the data at +8. */
void *func_00225D94(CWorld *arg0, s32 arg1) {
    s32 *t = arg0->m_pCurves;
    return (s8 *)t + (arg1 * *t + 8);
}
