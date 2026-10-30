#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 x20;
    /* 0x22 */ u8 pad22[0xE];
} ChanState; /* size 0x30 */

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ ChanState *chanState;
} SeqPlayer;

void func_00293E30(SeqPlayer *seqp, s16 chan, u8 value) {
    seqp->chanState[chan].x20 = value;
}
