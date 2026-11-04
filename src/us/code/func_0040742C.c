#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

/* D_8012F978 sits one word into the boss state: the word before it is the other actor. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *actor;
    /* 0x04 */ u8 pad4[0x64 - 0x4];
    /* 0x64 */ s32 mode;
} BossState;

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern BossState D_8012F978;

void func_0040742C(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        func_00243414((s32) D_8012F978.actor, (s32) &D_8012F978.actor->m_AI, 1);
        func_00243414((s32) (&D_8012F978.actor)[-1], (s32) &(&D_8012F978.actor)[-1]->m_AI, 1);
        D_8012F978.mode = 8;
        D_800F1C40[0] |= 0x300;
        D_800F1F50[0] |= 0x300;
        func_00243414(arg0, (s32) ai, 0x12);
    }
}
