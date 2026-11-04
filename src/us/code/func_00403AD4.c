#include "common.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 id;
} Obj00403AD4;

void func_0021034C(s32 a0, f32 r, f32 g, f32 b);

void func_00403AD4(s32 a0, Obj00403AD4 *obj) {
    f32 level = 1.0f - D_8012F608.partTimer * 0.011111111f;

    switch (obj->id) {
        case 0x6C:
            if (D_8012F608.partActive && D_8012F608.part == 0) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x68:
            if (D_8012F608.partActive && D_8012F608.part == 2) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x33:
            if (D_8012F608.partActive && D_8012F608.part == 3) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x29:
            if (D_8012F608.partActive && D_8012F608.part == 6) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x1C:
            if (D_8012F608.partActive && D_8012F608.part == 4) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x2E:
            if (D_8012F608.partActive && D_8012F608.part == 5) {
                func_0021034C(a0, level, level, level);
            }
            break;
    }
}
