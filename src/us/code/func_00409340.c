#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Boss fight setup: finds the six level actors with id 0x1DB1 and the one
 * with 0x1DB2, installs the AI callbacks and resets the 15 attack slots. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
} Obj;

extern s32 D_80110048;
extern u8 D_800F7078[];
extern u32 D_800F1F10[];
extern u8 D_00431F00[];

s32 func_00225F6C(void *, s32, s32, s32, CGameObjectInstance **, s32);
s32 func_00225EBC(void *, s32, s32);
void func_00243414(Obj *, CAI *, s32);
void func_002362E4(void *, Obj *);
void func_0040BC64();
void func_0040BEE4();
void func_0040BFF0();
void func_0025E4B8();
extern u8 func_00408FF8[];

void func_00409340(Obj *obj, CAI *ai) {
    MotherBossState *s = &D_8012FB70;
    s32 i;

    D_80110048 = 0x1DB1;
    s->unk680 = 0;
    s->unk684 = 0;
    s->partCount = func_00225F6C(D_800F7078, 0x650, 10, 0x1DB1, (CGameObjectInstance **)s->parts, 6);
    for (i = 0; i < s->partCount; i++) {
        ((CGameObjectInstance *)s->parts[i])->m_AI.m_Mother.m_bActive = 1;
    }
    s->partsLeft = s->partCount;
    func_00225F6C(D_800F7078, 0x650, 10, 0x1DB2, (CGameObjectInstance **)&s->controller, 1);
    ((CGameObjectInstance *)s->controller)->m_AI.m_Mother.m_bActive = 1;
    s->intel = (void *)func_00225EBC(D_800F7078, 1, 0x1DB1);
    ai->m_pModeTable = D_00431F00;
    ai->m_pfStep = func_0040BC64;
    ai->m_pfHurt = func_00408FF8 + 8;
    ai->m_pfEffects = func_0025E4B8;
    ai->m_pfCollision = func_0040BFF0;
    ai->m_pfDraw = func_0040BEE4;
    func_00243414(obj, ai, 0);
    D_8012FB70.obj = obj;
    D_800F1F10[0] &= ~0x300;
    obj->flags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        func_002362E4(D_8012FB70.slots[i].anim, obj);
    }
}
