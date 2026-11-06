#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/scene.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204D2C.c, func_00204D2C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B01Pair {
    s32 a;
    s32 b;
} B01Pair;
extern u8 gEngineApp_m_SwooshSystem[];
void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, B01Pair extra);
void CScene__SetInstanceSavedBit(void *mgr, CGameObjectInstance *actor, s32 flag);
void GameObject_FireTriggers(CGameObjectInstance *actor, s32 flags, CGameObjectInstance *actor2);
void SwooshSet_FreeOwner(void *p, CGameObjectInstance *actor);
void Door_CloseEnter(CGameObjectInstance *actor) {
    B01Pair extra;
    extra.a = 0;
    CScene__SetInstanceSavedBit(&gScene, actor, 0);
    GameObject_FireTriggers(actor, 0x8000, actor);
    SwooshSet_FreeOwner(gEngineApp_m_SwooshSystem, actor);
    if (gScene.m_Mode != 4) {
        ImpactEvent_Call(actor, actor, 7, actor->header.m_vPos, extra);
        actor->m_dwFlags |= 0x8000000;
    }
}
