#include "common.h"

s32 func_002017D4(s32, s32);                    /* extern */

/* The world: the saved-actor bitmap sections (see func_00220260). */
typedef struct World {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 bitSections;
} World;

void func_00224C98(World *arg0, s32 arg1) {
    s32 temp_v0;
    u8 *temp_v0_2;

    temp_v0 = func_002017D4(arg0->bitSections, 1);
    func_002017D4(temp_v0, 0);
    temp_v0_2 = func_002017D4(temp_v0, 1) + arg1;
    *temp_v0_2 -= 1;
}
