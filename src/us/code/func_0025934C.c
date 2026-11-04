#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ CGameObjectInstance actor;
    /* 0x288 */ u8 pad288[0xA64 - 0x288];
    /* 0xA64 */ u32 buttons;
    /* 0xA68 */ u8 padA68[0xC04 - 0xA68];
    /* 0xC04 */ s32 anim;
} Player;

s32 func_00219F74(void *, s32, s32);        /* extern */
s32 func_0021E9F8(void *, s32, s32);        /* extern */

void func_0025934C(CGameObjectInstance *obj) {
    Player *temp_s0;

    temp_s0 = (Player *) obj->m_AI.m_pParent;
    if (temp_s0->buttons & 0x2000) {
        temp_s0->anim = 0x1130;
        func_0021E9F8(temp_s0, func_00219F74(temp_s0, 0x1130, -1), 0);
        temp_s0->actor.m_CurrentAnimation.m_bCycleCompleted = 0;
    }
}
