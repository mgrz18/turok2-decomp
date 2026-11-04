#include "common.h"
#include "structs/game_object.h"


/* Sums `value` over the group, and separately over the members still alive with flag 0x100. */
void func_0040F1D8(void *m, s32 count, CGameObjectInstance **group, s32 *total, s32 *active) {
    CGameObjectInstance *a;
    s32 i;

    for (i = 0; i < count; i++) {
        a = group[i];
        *total += a->m_AI.m_InitialHealth;
        if (a->m_AI.m_Health != 0 && (a->m_dwFlags & 0x100)) {
            *active += a->m_AI.m_InitialHealth;
        }
    }
}
