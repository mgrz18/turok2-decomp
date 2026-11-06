#include "common.h"

s32 CCache__Request(s32, s32, s32, s32, s32, s32 *, s32 *, s32); /* extern */
extern s32 Texture_DecompressCallback;
extern s32 D_800A51F0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} Pair;

void Texture_RequestSync(Pair *arg0) {
    CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &D_800A51F0, 1);
}
