#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x54C];
    /* 0x54C */ s32 unk54C;
} Owner;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ Owner *owner;
} Player;

s32 func_00253FE0(Player *arg0) {
    s32 var_v0;
    Owner *temp_v1;

    temp_v1 = arg0->owner;
    var_v0 = 0;
    if (temp_v1 != NULL) {
        var_v0 = temp_v1->unk54C != 0;
    }
    return var_v0;
}
