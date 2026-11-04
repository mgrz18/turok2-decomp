#include "common.h"

/* The sound context (SndCtx in func_002726DC). */
typedef struct {
    /* 0x00 */ u8 pad0[0x84];
    /* 0x84 */ u8 player[0x58]; /* ALSndPlayer */
    /* 0xDC */ s16 snd[16];
} SndCtx;

/* A sound voice: its slot in the context, the context and its volume scale. */
typedef struct {
    /* 0x00 */ s32 slot;
    /* 0x04 */ u8 pad4[0xB0 - 4];
    /* 0xB0 */ SndCtx *ctx;
    /* 0xB4 */ u8 padB4[0xC8 - 0xB4];
    /* 0xC8 */ f32 volumeScale;
} SndVoice;

s32 func_00293E70(s32, s32);
s32 func_00293E60(s32, s16);                    /* extern */

void func_002746AC(SndVoice *arg0, s32 arg1) {
    SndCtx *temp_s0;
    s32 temp_s0_2;

    temp_s0 = arg0->ctx;
    temp_s0_2 = (s32) temp_s0->player;
    func_00293E60(temp_s0_2, temp_s0->snd[arg0->slot]);
    func_00293E70(temp_s0_2, (s32) (s16) (s32) ((f32) arg1 * arg0->volumeScale));
}
