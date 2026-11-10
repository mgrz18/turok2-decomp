#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
} Arg0;

sptr func_00226160(s32 *, u8, s16);          /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr CPathTrack__GetCurrentPoint(Arg0 *arg0) {
    return func_00226160(&gScene, arg0->unk1, arg0->unk2);
}
#else
void CPathTrack__GetCurrentPoint(Arg0 *arg0) {
    func_00226160(&gScene, arg0->unk1, arg0->unk2);
}
#endif
