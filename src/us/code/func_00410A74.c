/* WRONG CUT: func_00410A74 really runs 0x410A74-0x410BB4 (0x140 bytes); splat's func_00410B5C (0x58 bytes) is its tail
 * (the clamp and epilogue), not a function. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80400000_15.CPP, func_8040170C_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 value;
    /* 0x10 */ f32 time;
} Key; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ void *owner;
    /* 0x04 */ void *resource;
} Cinema;

/* The two adjacent track constants are read at base and base+4. */
typedef struct {
    f32 amplitude;
    f32 unit;
} TrackConstants;

extern Cinema *D_800C1BB0;
extern f32 D_800C1BC8;
extern TrackConstants D_800C0988;
extern f32 D_800C0990[];

extern s32 *func_002017D4(void *table, s32 index);

/* Evaluates the scalar key track 1 of the current cinema at time t, then rescales and clamps it to [0, 1]. */
f32 func_00410A74(f32 t) {
    s32 *track;
    Key *key;
    s32 count;
    f32 u;
    f32 v;

    track = func_002017D4(D_800C1BB0->resource, 1);
    count = track[1];
    key = (Key *) (track + 2);
    if (count == 0) {
        v = 0.0f;
    } else if (t <= key[0].time) {
        v = key[0].value;
    } else if (key[count - 1].time <= t) {
        v = key[count - 1].value;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        v = key[-1].value * (D_800C0988.unit - u) + key->value * u;
    }
    v *= 2.0f * D_800C1BC8 + D_800C0990[0];
    v -= D_800C1BC8;
    if (D_800C0990[0] < v) {
        v = D_800C0990[0];
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    return v;
}
