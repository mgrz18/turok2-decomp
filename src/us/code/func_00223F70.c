#include "common.h"
#include "structs/world.h"

/* Element i of a table whose first word is the element size, data at +8. */
void *CScene__GetLight(CWorld *arg0, s32 arg1) {
    s32 *t = arg0->m_pLights;
    return (s8 *)t + (arg1 * *t + 8);
}
