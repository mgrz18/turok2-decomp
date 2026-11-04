#include "common.h"

/* Two words at D_8012FA24; the pointer just before them (D_8012FA20) picks
 * the data for the given mode. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} ModePair;

extern u8 D_00431624[];
extern u8 D_00431660[];
extern u8 D_0043169C[];
extern u8 D_00431700[];
extern ModePair D_8012FA24;

void func_00407158(s32 arg0) {
    D_8012FA24.unk0 = -1;
    D_8012FA24.unk4 = -1;
    if (arg0 == 0) {
        ((u8 **) &D_8012FA24)[-1] = NULL;
    }
    if (arg0 == 1) {
        ((u8 **) &D_8012FA24)[-1] = D_00431660;
    }
    if (arg0 == 4) {
        ((u8 **) &D_8012FA24)[-1] = D_00431700;
    }
    if (arg0 == 2) {
        ((u8 **) &D_8012FA24)[-1] = D_00431624;
    }
    if (arg0 == 3) {
        ((u8 **) &D_8012FA24)[-1] = D_0043169C;
    }
}
