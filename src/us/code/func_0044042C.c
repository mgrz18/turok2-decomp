#include "common.h"
#include "structs/game_object.h"

/* The boss state at D_8012F978: an object, and a word at 0x64; the word
 * before it (D_8012F974) is a second object. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *obj;
    /* 0x04 */ u8 pad4[0x64 - 0x4];
    /* 0x64 */ s32 unk64;
} BossObjects;

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern BossObjects D_8012F978;

void func_0044042C(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        func_00243414(D_8012F978.obj, &D_8012F978.obj->m_AI, 1);
        func_00243414(((CGameObjectInstance **) &D_8012F978)[-1], &((CGameObjectInstance **) &D_8012F978)[-1]->m_AI, 1);
        D_8012F978.unk64 = 8;
        D_800F1C40[0] |= 0x300;
        D_800F1F50[0] |= 0x300;
        func_00243414(arg0, arg1, 0x12);
    }
}
