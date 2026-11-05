#include "common.h"
#include "structs/world.h"

/* Rumble i: the table starts with the element size, the data at +8. */
void *func_00226DCC(CWorld *arg0, s32 arg1) {
    s32 *t = arg0->m_pRumbles;
    return (s8 *)t + (arg1 * *t + 8);
}
