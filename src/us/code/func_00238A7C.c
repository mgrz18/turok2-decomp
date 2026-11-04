#include "common.h"
#include "structs/game_object.h"

typedef struct Player {
    u8 pad0[0x51C];
    void *unk51C;       /* 0x51C */
} Player;


typedef struct SoundPair {
    s32 sound;          /* 0x0 */
    s32 effect;         /* 0x4 */
} SoundPair;

typedef struct EventParams {
    s32 index;          /* 0x0 */
    s32 unk4;           /* 0x4 */
} EventParams;

extern u8 D_801198B0[];
extern SoundPair D_800B69D8[];
extern void func_0027FC2C(void *arg0, void *arg1, s32 arg2);
extern void func_0027580C(s32 arg0);

void func_00238A7C(s32 arg0, CGameObjectInstance *actor, s32 arg2, CVector3 pos, EventParams params) {
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        func_0027FC2C(D_801198B0, ((Player *)actor->m_AI.m_pParent)->unk51C, D_800B69D8[params.index].sound);
    } else {
        func_0027FC2C(D_801198B0, D_801198B0 + 0x40, D_800B69D8[params.index].sound);
    }
    if (D_800B69D8[params.index].effect != 0) {
        func_0027580C(D_800B69D8[params.index].effect);
    }
}
