#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E77C.c, func_0022E77C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"
extern u8 D_8010FF34[];
void *CDLightSet__Alloc(void *tbl, s32 id);
void CGameObjectInstance__LocalToWorld(CVector3 *out, CGameObjectInstance *actor, CVector3 dir);
void CVector3__Add(CVector3 *out, CVector3 *a, void *b);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);
void Particle_AttachToActor(CGameObjectInstance *actor, void **slot, s32 id, f32 z)
{
    void *p;
    CVector3 dir;
    CVector3 out;
    p = *slot;
    if (p == ((void *)0)) {
        p = CDLightSet__Alloc(D_8010FF34, id);
        if (p == ((void *)0)) {
            return;
        }
    }
    *slot = p;
    dir.x = 0;
    dir.y = 0;
    dir.z = z;
    CGameObjectInstance__LocalToWorld(&out, actor, dir);
    CVector3__Add(&out, &out, &actor->header.m_vPos);
    (*(s16 *)((u8 *)(p) + (0x16))) = 1;
    (*(s16 *)((u8 *)(p) + (0x10))) = out.x;
    (*(s16 *)((u8 *)(p) + (0x12))) = out.y + GameObject_GetCollisionHeight(actor) * 0.5f;
    (*(s16 *)((u8 *)(p) + (0x14))) = out.z;
    (*(f32 *)((u8 *)(p) + (0xC))) = 1.0f;
}
