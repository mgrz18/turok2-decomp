/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_802643E8_de, with Turok 2's layouts. */
#include "common.h"

/* Controller profile, stride 0x224. */
typedef struct {
    u8 pad0[0xB4];
    u32 bits; /* 0xB4 */
} Controller;

void func_00275624(s32 sound);

extern s32 D_800B6CF8; /* menu sounds enabled */

s32 func_00285748(Controller *ctrl) {
    if (ctrl->bits & 0x4000) {
        if (D_800B6CF8 != 0) {
            func_00275624(0x12C);
        }
        return 1;
    }
    return 0;
}
