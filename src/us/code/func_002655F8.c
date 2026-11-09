#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204C68.c, func_00204C68.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B01Pair {
    s32 a;
    s32 b;
} B01Pair;
extern u8 gEngineApp_m_SwooshSystem[];
void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, B01Pair extra);
void SwooshSet_FreeOwner(void *p, CGameObjectInstance *actor);
void Door_OpenEnter(CGameObjectInstance *actor, CAI *state) {
    B01Pair extra;
    extra.a = 0;
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 0x4000, actor);
    state->m_ConditionTimer = 0.0f;
    SwooshSet_FreeOwner(gEngineApp_m_SwooshSystem, actor);
    if (gScene.m_Mode != 4) {
        ImpactEvent_Call(actor, actor, 6, actor->header.m_vPos, extra);
        actor->m_dwFlags |= 0x8000000;
    }
}
