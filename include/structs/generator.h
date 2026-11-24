#ifndef STRUCTS_GENERATOR_H
#define STRUCTS_GENERATOR_H

/* The generator state an actor's intelligence entry holds at +0x14
 * (Generator_Advance, Generator_WaitUpdate; Rage Wars func_00205F18). */
#include "common.h"
#include "structs/word.h"

typedef union B01FH { f32 f; s32 i; s16 h; } B01FH;
typedef struct B01Info {
                u32 flags;
                s32 unk4;
                s16 unk8;
                s16 unkA;
                CWord unkC;
                B01FH unk10;
                s16 unk14;
                s16 unk16;
                s16 unk18;
                u8 unk1A;
                u8 unk1B;
                u8 pad1C[0x8];
                u32 flags24;
                u8 pad28[0x4];
                s32 unk2C;
                s32 unk30;
                CWord unk34;
                s32 unk38;
                s32 unk3C;
                B01FH unk40;
                u8 pad44[0x4];
                f32 unk48;
                f32 unk4C;
                u8 pad50[0x1C];
                f32 unk6C;
} B01Info;

#endif
