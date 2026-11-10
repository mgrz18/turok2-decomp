#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x0 */ s8 kind;
    /* 0x1 */ s8 value;
    /* 0x2 */ s16 id;
} Arg0;

#ifdef TARGET_PORT
struct CVector3;
s16 func_002261A8(void *scene, s32 index, struct CVector3 *pt);
#else
s16 func_002261A8(s32 *);                       /* extern */
#endif

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS func_002261A8 gets this function's
 * $a1 (the path index) and $a2 (the position, from the caller func_00219BC0) untouched; the
 * host takes and passes them. */
void CPathTrack__StartAtNearestPoint(Arg0 *arg0, s8 arg1, struct CVector3 *pt) {
#else
void CPathTrack__StartAtNearestPoint(Arg0 *arg0, s8 arg1) {
#endif
    arg0->kind = 2;
    arg0->value = arg1;
#ifdef TARGET_PORT
    arg0->id = func_002261A8(&gScene, arg1, pt);
#else
    arg0->id = func_002261A8(&gScene);
#endif
}
