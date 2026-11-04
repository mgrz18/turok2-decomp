#include "common.h"

/* What this reads of the state at D_800F56B0. */
typedef struct {
    /* 0x000 */ s32 _unk0;
    /* 0x004 */ s32 m_bA;
    /* 0x008 */ s32 m_bB;
    /* 0x00C */ u8 _pad[0x1A8];
    /* 0x1B4 */ s32 m_Value;
} State800F56B0;

/* ControllerProfile (structs/controller.h) seen from its previousButtons:
 * the ROM addresses the four through D_800F5F6C = D_800F5EC0 + 0xAC, so this
 * file keeps that symbol and this local view. */
typedef struct {
    /* 0x00 */ s32 m_Prev;     /* previousButtons */
    /* 0x04 */ s32 m_Held;     /* buttons */
    /* 0x08 */ s32 m_Pressed;  /* buttonsSingle */
    /* 0x0C */ s32 m_unkC;     /* buttonsSingleNoHold */
    /* 0x10 */ s32 m_unk10;    /* buttonsDouble */
    /* 0x14 */ s32 m_unk14;    /* buttonsRepeat */
    /* 0x18 */ u8 _pad[0x224 - 0x18];
} PadState;

extern State800F56B0 D_800F56B0;
extern PadState D_800F5F6C[4];

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
            D_800F5F6C[i].m_unkC = 0;
            D_800F5F6C[i].m_unk10 = 0;
            D_800F5F6C[i].m_unk14 = 0;
            D_800F5F6C[i].m_Pressed = (D_800F5F6C[i].m_Prev ^ D_800F5F6C[i].m_Held) & D_800F5F6C[i].m_Held;
        }
    }
}
