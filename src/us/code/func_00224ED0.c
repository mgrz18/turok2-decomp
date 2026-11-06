#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BFB4.c, func_0028BFB4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
CPooledLevelActor *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 *refCount, s32 a, CVector3 z, CVector3 v, s32 b, f32 f);
void CScene__SpawnPoolActor(CScene *lv, s32 a1, CVector3 v, s32 x, f32 f) {
    CVector3 z;
    z.x = 0;
    z.y = 0;
    z.z = 0;
    CLevelActorPool__Spawn(&lv->m_ActorPool, NULL, x, z, v, a1, f);
}
