/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80206258.CPP, func_802063EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * The out-of-line copy of func_00262D74.c's spawnObject. */

#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/game.h"

void Generator_SpawnObject(CGameObjectInstance *actor, CAI *ai, CVector3 pos, CVector3 scale, sptr region, s32 type, s32 *model) {
    if (CInstancePool__Spawn(&gScene.m_InstancePool, type, 0, model, (void *)region, pos, actor->m_RotY, actor->m_vVelocity, scale, &ai->m_Generator.m_nActive) != 0) {
        ai->m_Generator.m_nRemaining -= 1;
    }
}
