#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 unk4;
} Unk0029B150;

extern Unk0029B150 *D_800B8920;   /* the default when arg0 is NULL */

s32 osGetThreadPri(Unk0029B150 *arg0) {
    Unk0029B150 *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = D_800B8920;
    }
    return var_a0->unk4;
}
