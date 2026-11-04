#include "common.h"

typedef struct {
    /* 0x0 */ s8 kind;
    /* 0x1 */ s8 value;
    /* 0x2 */ s16 id;
} Arg0;

s16 func_002261A8(s32 *);                       /* extern */
extern s32 D_800F7078;

void func_00284760(Arg0 *arg0, s8 arg1) {
    arg0->kind = 2;
    arg0->value = arg1;
    arg0->id = func_002261A8(&D_800F7078);
}
