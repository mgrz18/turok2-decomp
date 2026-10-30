#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ f32 angle;
} Anim;

typedef struct {
    /* 0x00 */ Anim *anim;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 type;
} Bone;

extern void func_00210894(void *mtx, f32 angle);

void func_00259A4C(void *mtx, Bone *bone) {
    f32 angle = bone->anim->angle;

    switch (bone->type) {
    case 1:
        func_00210894(mtx, -angle);
        break;
    case 12:
        func_00210894(mtx, angle + angle);
        break;
    }
}
