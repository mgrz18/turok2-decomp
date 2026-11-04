#include "common.h"

/* The sound context (SndCtx in func_002726DC). */
typedef struct {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ u8 player[0x58]; /* ALSndPlayer */
    /* 0x0DC */ s16 snd[16];
    /* 0x0FC */ u8 padFC[8];
    /* 0x104 */ s32 frame;
} SndCtx;

/* A sound voice (0xCC bytes, in an array at 4 of its owner): its slot in the context,
 * the context and its volume scale. */
typedef struct {
    /* 0x00 */ s32 slot;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x10 - 8];
    /* 0x10 */ s32 frame;
    /* 0x14 */ u8 pad14[0x50 - 0x14];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0xAC - 0x54];
    /* 0xAC */ s32 stopped;
    /* 0xB0 */ SndCtx *ctx;
    /* 0xB4 */ u8 padB4[0xC8 - 0xB4];
    /* 0xC8 */ f32 volumeScale;
} SndVoice; /* size 0xCC */

/* What owns the voices: an array of them at 4. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ SndVoice voices[1];
} SndVoiceTable;

s32 func_00293EC0(s32);

s32 func_00293580(void *);                          /* extern */
s32 func_00293E60(void *, s16);                 /* extern */

void func_00273DD8(s32 arg0, s16 arg1) {
    SndCtx *temp_a0;
    SndVoice *var_s0;
    void *var_s1;

    /* voice arg1 of the table at arg0 */
    var_s0 = (SndVoice *) ((arg1 * 0xCC) + arg0);
    var_s0 = (SndVoice *) &((SndVoiceTable *) var_s0)->voices[0];
    temp_a0 = var_s0->ctx;
    var_s0->stopped = 1;
    var_s0->unk50 = 0;
    var_s1 = temp_a0->player;
    if (var_s0->frame != temp_a0->frame) {
        func_00293E60(var_s1, temp_a0->snd[var_s0->slot]);
        if (func_00293580(var_s1) != 0) {
            func_00293EC0((s32) var_s1);
        }
        var_s0->unk4 = -1;
    }
}
