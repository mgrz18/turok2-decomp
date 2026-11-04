#include "common.h"
#include "rw/abu_protos.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ s32 flags;
} Target;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ Target *target;
} Owner;

extern s32 D_8011ACE4;

void func_00455CD0(Owner *arg0) {
    Target *temp_a0;
    Target *temp_v0;

    if (D_8011ACE4 != 0) {
        temp_v0 = arg0->target;
        temp_v0->flags = (s32) (temp_v0->flags | 0x01000000);
        return;
    }
    temp_a0 = arg0->target;
    temp_a0->flags = (s32) (temp_a0->flags & 0xFEFFFFFF);
}
