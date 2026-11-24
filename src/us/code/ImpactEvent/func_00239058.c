#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002683F0.c, func_002683F0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
#include "structs/particle_event.h"

void ImpactEvent_ApplyBlast(CGameObjectInstance *victim, CGameObjectInstance *source, s32 hit, CVector3 pos, CImpactBlast blast, s32 falloff, s32 push, u32 flags);
/* Static inline in the Rage Wars source file (dropped by the importer);
 * Turok 2 passes 0x400000 where Rage Wars passes 0x200. */
static inline void applyAreaBlast(CGameObjectInstance *a, sptr b, s32 c, CVector3 pos, CImpactBlast blk) {
    ImpactEvent_ApplyBlast(a, (CGameObjectInstance *) b, c, pos, blk, 1, 0, 0x400000);
}
void ImpactEvent_BlastSameHeight(CGameObjectInstance *a, sptr b, s32 c, CVector3 pos, CImpactBlast blk) {
    f32 d;
    d = a->header.m_vPos.y - pos.y;
    if (d < 0.0f ? -d <= 76.799995f : d <= 76.799995f) {
        applyAreaBlast(a, b, c, pos, blk);
    }
}
