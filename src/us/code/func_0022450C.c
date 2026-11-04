#include "common.h"
#include "rw/abu_protos.h"

char *func_002017D4(s32 *, s32);
s32 func_00201818(s32, s32);

/* The world: the saved bitmap sections and the current level (see func_00220260). */
typedef struct World {
    /* 0x00000 */ u8 pad0[0x70];
    /* 0x00070 */ s32 *bitSections2;
    /* 0x00074 */ u8 pad74[0x18FA8 - 0x74];
    /* 0x18FA8 */ s32 level;
} World;

/* Tests a bit of the current level's second saved bitmap. */
s32 func_0022450C(World *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_s0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg1;
    temp_s0 = arg0->level;
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(arg0->bitSections2, 0), temp_s0), 0);
    func_002017D4(temp_v0, 0);
    func_00201818((s32) temp_v0, 1);
    temp_a0 = func_002017D4(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}
