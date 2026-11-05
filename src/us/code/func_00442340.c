#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module init: find the module actors (0x1DB1) and the controller (0x1DB2), set up the AI (mode table and callbacks),
 * then attach every slot to the object. */

extern s32 D_80110048;
extern s32 D_800F1F10[];
extern u8 D_800F7078[];
extern u8 D_00431E20[];
extern s32 func_00225F6C(void *world, s32 key0, s32 key1, s32 key2, void *out, s32 max);
extern void *func_00225EBC(void *world, s32 a1, s32 id);
extern void func_002362E4(void *slot, void *obj);
extern void func_00243414(void *obj, CAI *ai, s32 event);
extern void func_0025E4B8();
extern void func_00408FF8();
extern void func_0040BC64();
extern void func_0040BEE4();
extern void func_0040BFF0();

void func_00442340(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    D_80110048 = 0x1DB1;
    state->pickupTimer = 0;
    state->dropTimer = 0;
    state->partCount = func_00225F6C(D_800F7078, 0x650, 10, 0x1DB1, state->parts, 6);
    for (i = 0; i < state->partCount; i++) {
        state->parts[i]->m_AI.m_Mother.m_bActive = 1;
    }
    state->partsLeft = state->partCount;
    func_00225F6C(D_800F7078, 0x650, 10, 0x1DB2, &state->controller, 1);
    state->controller->m_AI.m_Mother.m_bActive = 1;
    state->intel = func_00225EBC(D_800F7078, 1, 0x1DB1);
    ai->m_pModeTable = (struct CAIModeInfo *) D_00431E20;
    ai->m_pfStep = func_0040BC64;
    ai->m_pfHurt = (u8 *)func_00408FF8 + 8;
    ai->m_pfEffects = func_0025E4B8;
    ai->m_pfCollision = func_0040BFF0;
    ai->m_pfDraw = func_0040BEE4;
    func_00243414(obj, ai, 0);
    D_8012FB70.obj = obj;
    D_800F1F10[0] &= ~0x300;
    obj->m_dwFlags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        func_002362E4(&D_8012FB70.slots[i].anim, obj);
    }
}
