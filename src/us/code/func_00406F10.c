#include "common.h"

typedef struct {
    /* 0x000 */ s32 kind;
    /* 0x004 */ u8 pad4[0x3F4 - 0x4];
    /* 0x3F4 */ s32 *script;
    /* 0x3F8 */ s32 unk3F8;
    /* 0x3FC */ s32 unk3FC;
} State;

extern s32 D_00431008;
extern s32 D_00431094;
extern s32 D_004310E4;
extern s32 D_00431198;
extern s32 D_00431238;
extern s32 D_00431300;
extern State D_8012F608;

void func_00406F10(s32 arg0) {
    if (arg0 == 2) {
        if (D_8012F608.kind == 1) {
            D_8012F608.script = &D_00431094;
        }
        if (D_8012F608.kind == arg0) {
            D_8012F608.script = &D_00431198;
        }
        if (D_8012F608.kind == 3) {
            D_8012F608.script = &D_00431300;
        }
        D_8012F608.unk3F8 = -1;
        D_8012F608.unk3FC = -1;
    }
    if (arg0 == 1) {
        if (D_8012F608.kind == arg0) {
            D_8012F608.script = &D_00431008;
        }
        if (D_8012F608.kind == 2) {
            D_8012F608.script = &D_004310E4;
        }
        if (D_8012F608.kind == 3) {
            D_8012F608.script = &D_00431238;
        }
        D_8012F608.unk3F8 = -1;
        D_8012F608.unk3FC = -1;
    }
}
