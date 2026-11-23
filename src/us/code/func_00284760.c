#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x0 */ s8 kind;
    /* 0x1 */ s8 value;
    /* 0x2 */ s16 id;
} Arg0;

#ifdef TARGET_PORT
struct CVector3;
s16 CScene__FindNearestPathPoint(void *scene, s32 index, struct CVector3 *pt);
#else
s16 CScene__FindNearestPathPoint(s32 *);                       /* extern */
#endif

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__FindNearestPathPoint gets this function's
 * $a1 (the path index) and $a2 (the position, from the caller CGameObjectInstance__Construct) untouched; the
 * host takes and passes them. */
void CPathTrack__StartAtNearestPoint(Arg0 *arg0, s8 arg1, struct CVector3 *pt) {
    arg0->kind = 2;
    arg0->value = arg1;
#ifdef TARGET_PORT
    arg0->id = CScene__FindNearestPathPoint(&gScene, arg1, pt);
#else
    arg0->id = CScene__FindNearestPathPoint(&gScene);
#endif
}
