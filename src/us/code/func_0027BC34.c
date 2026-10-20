#include "common.h"

/* What this reads of the state at D_800F56B0. */
typedef struct {
    /* 0x000 */ s32 _unk0;
    /* 0x004 */ s32 m_bA;
    /* 0x008 */ s32 m_bB;
    /* 0x00C */ u8 _pad[0x1A8];
    /* 0x1B4 */ s32 m_Value;
} State800F56B0;

extern State800F56B0 D_800F56B0;

/* D_800F56B0's value at 0x1B4 while either of its flags at 4 and 8 is set,
 * else 0. */
s32 func_0027BC34(void) {
    State800F56B0 *state = &D_800F56B0;

    if (state->m_bA != 0 || state->m_bB != 0) {
        return state->m_Value;
    }
    return 0;
}
