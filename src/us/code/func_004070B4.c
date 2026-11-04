#include "common.h"

/* D_8012FA10: a small state block; the word before it (D_8012FA0C) is the
 * script pointer chosen here. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s8 unk8;
} State;

extern s32 D_004313B4;
extern s32 D_00431424;
extern s32 D_004314CC;
extern s32 D_00431558;
extern s32 D_00431590;
extern State D_8012FA10;

#define SCRIPT (((s32 **) &D_8012FA10)[-1])

void func_004070B4(s32 arg0) {
    D_8012FA10.unk0 = -1;
    D_8012FA10.unk8 = 1;
    D_8012FA10.unk4 = -1;
    if (arg0 == 0) {
        SCRIPT = NULL;
    }
    if (arg0 == 1) {
        SCRIPT = &D_004313B4;
    }
    if (arg0 == 2) {
        SCRIPT = &D_00431424;
    }
    if (arg0 == 3) {
        SCRIPT = &D_004314CC;
    }
    if (arg0 == 4) {
        SCRIPT = &D_00431558;
    }
    if (arg0 == 5) {
        SCRIPT = &D_00431590;
    }
}
