#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 a;
    /* 0x08 */ s32 b;
    /* 0x0C */ s32 c;
    /* 0x10 */ s32 d;
} Block00281C08; /* size 0x14 */

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 scale;
    /* 0x18 */ Block00281C08 blocks[3];
} Unk00281C08;

void func_00281C08(Unk00281C08 *arg0) {
    Block00281C08 *temp_a0;
    Block00281C08 *temp_v0;
    Block00281C08 *temp_v0_2;

    temp_v0 = &arg0->blocks[0];
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk10 = 0;
    arg0->scale = 1.0f;
    temp_v0->a = 0;
    temp_v0->b = 0;
    temp_v0->c = 0;
    temp_v0->d = 0;
    temp_v0_2 = &arg0->blocks[1];
    temp_a0 = &arg0->blocks[2];
    temp_v0_2->a = 0;
    temp_v0_2->b = 0;
    temp_v0_2->c = 0;
    temp_v0_2->d = 0;
    temp_a0->a = 0;
    temp_a0->b = 0;
    temp_a0->c = 0;
    temp_a0->d = 0;
}
