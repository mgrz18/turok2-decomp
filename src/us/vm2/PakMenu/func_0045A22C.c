#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x684];
    /* 0x684 */ s32 value;
} Slot;


void Pak_SetBufferSize_2(s32 arg0, s32 arg1) {
    ((Slot *) (gScene.unk_30 + arg0))->value = arg1;
}
