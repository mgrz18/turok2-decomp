#include "common.h"
#include "structs/game_object.h"

/* The parent of the object: a player, with a word at 0x1288 (0 lets func_00253DE0 run). */
typedef struct {
    /* 0x0000 */ u8 pad0[0x1288];
    /* 0x1288 */ s32 unk1288;
} Player;

s32 func_00253DE0(void *, s32);             /* extern */
s32 func_0027588C(s32);                     /* extern */
extern s32 D_800B6D20;

void func_002597E8(CGameObjectInstance *arg0, CAI *arg1) {
    Player *temp_a0;

    temp_a0 = (Player *) arg0->m_AI.m_pParent;
    if (temp_a0->unk1288 == 0) {
        if (arg1->m_Mode != 1) {
            func_00253DE0(temp_a0, 0x1A8);
        }
    }
    if ((arg1->m_Mode != 4) && (D_800B6D20 & 1)) {
        func_0027588C(0x1A9);
    }
}
