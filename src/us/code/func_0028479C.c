#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
} Arg0;

sptr CScene__GetPathPoint(s32 *, u8, s16);          /* extern */

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr CPathTrack__GetCurrentPoint(Arg0 *arg0) {
    return CScene__GetPathPoint(&gScene, arg0->unk1, arg0->unk2);
}
