#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* AI init dispatcher: the 0x44E..0x456 types have their own init in the paged
 * VM code; everything else gets the generic init. */

extern u8 D_800F7078[];

extern void func_00402888(CGameObjectInstance *actor, CAI *ai);
extern void func_00409340(CGameObjectInstance *actor, CAI *ai);
extern void func_00403CF8(CGameObjectInstance *actor, CAI *ai);
extern void func_0040D5C4(CGameObjectInstance *actor, CAI *ai);
extern void func_0040D6A0(CGameObjectInstance *actor, CAI *ai);
extern void func_0040D7A8(CGameObjectInstance *actor, CAI *ai);
extern void func_0040D8B0(CGameObjectInstance *actor, CAI *ai);
extern void func_00243414(CGameObjectInstance *actor, CAI *ai, s32 anim);
extern void func_0025E52C(CGameObjectInstance *actor, CAI *ai, f32 *pos, s32 arg3);
extern s32 func_00220408(void *mgr, CGameObjectInstance *actor);

void func_0025BE88(CGameObjectInstance *actor, CAI *ai) {
    u32 *defFlags = &actor->header.m_pIntelligence->m_dwTypeFlags;

    switch (actor->m_nTypeFlag) {
    case 0x451:
        func_00402888(actor, ai);
        break;
    case 0x44E:
        func_00409340(actor, ai);
        break;
    case 0x453:
        func_00403CF8(actor, ai);
        break;
    case 0x450:
        func_0040D5C4(actor, ai);
        break;
    case 0x454:
        func_0040D6A0(actor, ai);
        break;
    case 0x455:
        func_0040D7A8(actor, ai);
        break;
    case 0x456:
        func_0040D8B0(actor, ai);
        break;
    default:
        if (actor->m_nTypeFlag == 0x467) {
            func_00243414(actor, ai, 0x31);
        } else if (actor->m_dwFlags & 0x80000) {
            func_00243414(actor, ai, 2);
        } else {
            func_00243414(actor, ai, 0);
        }
        ai->m_Action = 0;
        break;
    }
    if (!(*defFlags & 0x8000)) {
        ai->m_Action = 1;
        func_0025E52C(actor, ai, &actor->header.m_vPos.x, 3);
    }
    if (func_00220408(D_800F7078, actor) == 1) {
        actor->m_dwFlags &= ~0x100;
    }
}
