#include "common.h"

void AI_UpdateTargets(sptr a0, sptr a1, void *out, s32 a3, s32 a4);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnClamped AI_TurnClamped_i
#endif
void AI_TurnClamped(sptr a0, sptr a1, f32 a2, s32 a3);

void MantidQueen_UpdateMode15(s32 a0, s32 a1) {
    u8 buf[0x88];

    AI_UpdateTargets(a0, a1, buf, 1, 1);
    AI_TurnClamped(a0, a1, 0.0174532961f, *(s32 *)(buf + 0x4C));
}
