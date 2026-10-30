/* Cut fixed in versions/cuts.us.txt: func_00271BA8 really is 0x271BA8-0x271BEC (0x44 bytes): splat split it into func_00271BA8, func_00271BBC (its found-return) and func_00271BC4 (its loop). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO_2/802591C0.CPP, func_80258FF4_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
    /* 0x10 */ u8 pad10[0xBC];
} Voice; /* 0xCC bytes */

typedef struct {
    /* 0x0000 */ u8 pad0[0x1DBC];
    /* 0x1DBC */ Voice voices[17];
} SoundContext;

/* Index of the first of the seventeen voices playing id, -1 if none (or id is -1). */
s32 func_00271BA8(SoundContext *ctx, s32 id) {
    Voice *slot;
    s32 i;

    slot = ctx->voices;
    if (id == -1) {
        return -1;
    }
    for (i = 0; i < 17; i++, slot++) {
        if (slot->id == id) {
            return i;
        }
    }
    return -1;
}
