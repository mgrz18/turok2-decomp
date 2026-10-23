#include "common.h"

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad04[0x5F];
    /* 0x63 */ s8 values[6];
} Func0021EBE4Def;

typedef struct {
    /* 0x000 */ u8 pad000[0x14];
    /* 0x014 */ Func0021EBE4Def *def;
    /* 0x018 */ u8 pad018[0x15C];
    /* 0x174 */ s8 mode;
    /* 0x175 */ u8 pad175;
    /* 0x176 */ s8 prevMode;
} Func0021EBE4Obj;

void func_0021EBE4(Func0021EBE4Obj *obj, s32 offset, s32 *outA, s32 *outB) {
    Func0021EBE4Def *def;
    s32 mode;

    def = obj->def;
    if (def->type == 1) {
        if (obj->mode == 60) {
            mode = obj->prevMode;
        } else {
            mode = obj->mode;
        }
        switch ((u32) (mode - 54)) {
            case 0:
                *outA = *outB = def->values[0];
                return;
            case 1:
                *outA = *outB = def->values[1];
                return;
            case 2:
                *outA = *outB = def->values[2];
                return;
            case 3:
                *outA = *outB = def->values[3];
                return;
            case 4:
                *outA = *outB = def->values[4];
                return;
            case 5:
                *outA = *outB = def->values[5];
                return;
        }
    }
    *outA = ((s8 *) def)[offset + 0xE];
    *outB = ((s8 *) def)[offset + 0x10];
}
