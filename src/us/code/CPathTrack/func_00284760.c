#include "common.h"
#include "globals/scene.h"
#include "structs/game_object.h"


#ifdef TARGET_PORT
struct CVector3;
s16 CScene__FindNearestPathPoint(void *scene, s32 index, struct CVector3 *pt);
#else
s16 CScene__FindNearestPathPoint(s32 *);                       /* extern */
#endif

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__FindNearestPathPoint gets this function's
 * $a1 (the path index) and $a2 (the position, from the caller CGameObjectInstance__Construct) untouched; the
 * host takes and passes them. */
void CPathTrack__StartAtNearestPoint(CPathTrack *arg0, s8 arg1, struct CVector3 *pt) {
    arg0->m_Mode = 2;
    arg0->m_nCurrentPathIndex = arg1;
#ifdef TARGET_PORT
    arg0->m_nCurrentPointIndex = CScene__FindNearestPathPoint(&gScene, arg1, pt);
#else
    arg0->m_nCurrentPointIndex = CScene__FindNearestPathPoint(&gScene);
#endif
}
