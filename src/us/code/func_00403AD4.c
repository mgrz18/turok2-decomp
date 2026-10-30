#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x3C0];
    /* 0x3C0 */ s32 phase;
    /* 0x3C4 */ s32 mask;
    /* 0x3C8 */ u8 unk3C8;
    /* 0x3C9 */ u8 done;
    /* 0x3CA */ u8 pad1[0x3E8 - 0x3CA];
    /* 0x3E8 */ f32 fade;
} ModeState00403AD4;

typedef struct {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 id;
} Obj00403AD4;

extern ModeState00403AD4 D_8012F608;
void func_0021034C(s32 a0, f32 r, f32 g, f32 b);

void func_00403AD4(s32 a0, Obj00403AD4 *obj) {
    f32 level = 1.0f - D_8012F608.fade * 0.011111111f;

    switch (obj->id) {
        case 0x6C:
            if (D_8012F608.done && D_8012F608.phase == 0) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x68:
            if (D_8012F608.done && D_8012F608.phase == 2) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x33:
            if (D_8012F608.done && D_8012F608.phase == 3) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x29:
            if (D_8012F608.done && D_8012F608.phase == 6) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x1C:
            if (D_8012F608.done && D_8012F608.phase == 4) {
                func_0021034C(a0, level, level, level);
            }
            break;
        case 0x2E:
            if (D_8012F608.done && D_8012F608.phase == 5) {
                func_0021034C(a0, level, level, level);
            }
            break;
    }
}
