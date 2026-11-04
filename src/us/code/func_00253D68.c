#include "structs/game_object.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022AE28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2: position 0x4 and region 0x10 (Rage Wars 0x8 / 0x14). */
/* Turok 2: LevelActor is 0x288 bytes, so the weapon actor follows at 0x288. */
typedef struct Shared_ActorView {
    CGameObjectInstance actor;
    CGameObjectInstance weapon;
} Shared_ActorView;
typedef struct Shared_World Shared_World;
extern Shared_World D_800F7078;
extern s32 func_00220A1C(Shared_World *, CVector3 *);
int func_00253D68(void *arg0, void *arg1) {
    int region;
    CVector3 *src = arg1;
    Shared_ActorView *view = arg0;
    region = ((int (*)(void *))func_00220A1C)(&D_800F7078);
    view->actor.header.m_vPos = *src;
    view->actor.header.m_pCurrentRegion = (struct CGameRegion *) region;
    view->weapon.header.m_vPos = *src;
    view->weapon.header.m_pCurrentRegion = (struct CGameRegion *) region;
    return region != 0;
}
