#include "common.h"
#include "structs/controller.h"

/* What this reads of the state at D_800F56B0. */
typedef struct {
    /* 0x000 */ s32 _unk0;
    /* 0x004 */ s32 m_bA;
    /* 0x008 */ s32 m_bB;
    /* 0x00C */ u8 _pad[0x1A8];
    /* 0x1B4 */ s32 m_Value;
} State800F56B0;

extern State800F56B0 D_800F56B0;

void func_0042F344(void);

static __inline__ s32 func_0027BC34(void) {
    State800F56B0 *state = &D_800F56B0;

    if (state->m_bA != 0 || state->m_bB != 0) {
        return state->m_Value;
    }
    return 0;
}

/* Reads the controllers and works out which buttons were just pressed. */
void func_0027BB80(void) {
    s32 i;

    if (func_0027BC34()) {
        func_0042F344();
        for (i = 0; i < 4; i++) {
            D_800F5EC0[i].buttonsSingleNoHold = 0;
            D_800F5EC0[i].buttonsDouble = 0;
            D_800F5EC0[i].buttonsRepeat = 0;
            D_800F5EC0[i].buttonsSingle = (D_800F5EC0[i].previousButtons ^ D_800F5EC0[i].buttons) & D_800F5EC0[i].buttons;
        }
    }
}
