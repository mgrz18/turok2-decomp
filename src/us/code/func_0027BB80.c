#include "common.h"
#include "structs/demo.h"

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

extern PadState D_800F5F6C[4];

void func_0042F344(void);

static __inline__ s32 func_0027BC34(void) {
    DemoPlayer *state = &D_800F56B0;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
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
