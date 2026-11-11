#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022AE28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2: position 0x4 and region 0x10 (Rage Wars 0x8 / 0x14). */
/* Turok 2: LevelActor is 0x288 bytes, so the weapon actor follows at 0x288. */
typedef struct Shared_ActorView {
    CGameObjectInstance actor;
    CGameObjectInstance weapon;
} Shared_ActorView;
typedef struct Shared_World Shared_World;
int CPlayer__SetPosition(void *arg0, void *arg1) {
    sptr region;
    CVector3 *src = arg1;
    Shared_ActorView *view = arg0;
#ifdef TARGET_PORT
    /* the region pointer-wide, and the position passed (round 9 x2): the ROM calls
     * CScene__FindRegionAt through an int-returning one-argument cast (the position stays in a1
     * from its own caller); on the host that cut the region to 32 bits: with --high-memory the
     * Mother's phase-1 cinema end (MotherBoss_OnPhase1CinemaDone) put the player in a truncated
     * region and the camera crashed */
    region = (sptr) CScene__FindRegionAt((struct CScene *) &gScene, (f32 *) src);
#else
    region = ((int (*)(void *))CScene__FindRegionAt)(&gScene);
#endif
    view->actor.header.m_vPos = *src;
    view->actor.header.m_pCurrentRegion = (struct CGameRegion *) region;
    view->weapon.header.m_vPos = *src;
    view->weapon.header.m_pCurrentRegion = (struct CGameRegion *) region;
    return region != 0;
}
