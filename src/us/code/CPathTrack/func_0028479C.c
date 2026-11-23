#include "common.h"
#include "globals/scene.h"
#include "structs/game_object.h"


sptr CScene__GetPathPoint(s32 *, u8, s16);          /* extern */

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr CPathTrack__GetCurrentPoint(CPathTrack *arg0) {
    return CScene__GetPathPoint(&gScene, arg0->m_nCurrentPathIndex, arg0->m_nCurrentPointIndex);
}
