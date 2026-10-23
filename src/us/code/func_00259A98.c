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

void func_00210A68(void *mtx, f32 angle);

void func_00259A98(void *mtx, Bone *bone) {
    if (bone->type == 2) {
        func_00210A68(mtx, -bone->anim->angle * 0.5f);
    }
}
