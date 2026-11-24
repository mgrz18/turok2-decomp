#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E77C.c, func_0022E77C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"
/* The port: on the scene's CDLightSet and CDLight members (the N64 offsets 0xC..0x16 of the
 * light are 0x18..0x22 on the host); the ROM light is a pointer. */
#include "structs/particle.h"
#include "globals/scene.h"
void *CDLightSet__Alloc(void *tbl, void *light);
CVector3 *CGameObjectInstance__LocalToWorld(CVector3 *, void *, CVector3);
void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);
f32 GameObject_GetCollisionHeight(void *);
void Particle_AttachToActor(CGameObjectInstance *actor, void **slot, void *id, f32 z) {
    CDLight *p;
    CVector3 dir;
    CVector3 out;

    p = *slot;
    if (p == NULL) {
        p = CDLightSet__Alloc(&gScene.m_DLightSet, id);
        if (p == NULL) {
            return;
        }
    }
    *slot = p;
    dir.x = 0;
    dir.y = 0;
    dir.z = z;
    CGameObjectInstance__LocalToWorld(&out, actor, dir);
    CVector3__Add(&out, &out, &actor->header.m_vPos);
    p->m_bEnabled = 1;
    p->x = out.x;
    p->y = out.y + GameObject_GetCollisionHeight(actor) * 0.5f;
    p->z = out.z;
    p->m_ColorFade = 1.0f;
}
