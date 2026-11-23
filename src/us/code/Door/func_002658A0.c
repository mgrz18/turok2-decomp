#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/particle_event.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80204E78.CPP, func_80204F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, CImpactEventArg extra);
void Door_CloseNow(CGameObjectInstance *actor) {
    CImpactEventArg extra;
    extra.a = 0;
    ImpactEvent_Call(actor, actor, 7, actor->header.m_vPos, extra);
    CScene__SetInstanceSavedBit(&gScene, actor, 0);
}
