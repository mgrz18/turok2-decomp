#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x684];
    /* 0x684 */ s32 unk684;
} Entry;


void Pak_SetBufferSize_2(s32 arg0, s32 arg1) {
    ((Entry *) (gScene.unk_30 + arg0))->unk684 = arg1;
}
