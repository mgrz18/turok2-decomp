/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E120.c, func_0022E120.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "globals/scene.h"


void *CScene__GetRegionInfo(void *tbl, s32 id);


s32 Actor_IsInRegionFlag40(CGameObjectInstance *actor)
{
    s32 id = ((s32)actor->header.m_pCurrentRegion);
    if (id != 0) {
        void *p = CScene__GetRegionInfo(&gScene, id);
        if (p != ((void *)0)) {
            if (*(u16 *)((u8 *)p + 0x52) & 0x40) {
                return 1;
            }
        }
    }
    return 0;
}
