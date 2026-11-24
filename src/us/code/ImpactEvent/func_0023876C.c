/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_2/802661FC_2.CPP, func_80267BC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/particle_event.h"

void ImpactEvent_ApplyBlast(CGameObjectInstance *victim, CGameObjectInstance *source, s32 hit, CVector3 pos, CImpactBlast blast, s32 falloff, s32 push, u32 flags);
void ImpactEvent_AreaDamage(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 0, 0x100000);
}
