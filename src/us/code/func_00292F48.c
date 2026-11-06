#include "common.h"

s32 __alSeqReadWord(s32);

/* MIDI file reader state (checks the "MThd" and "MTrk" chunk tags). */
typedef struct {
    /* 0x00 */ s32 data;
    /* 0x04 */ s32 trackStart;
    /* 0x08 */ s32 cursor;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 tickScale;
    /* 0x18 */ s16 division;
    /* 0x1A */ s16 unk1A;
} MidiReader;

s16 __alSeqReadShort(void *);                          /* extern */

void alSeqNew(MidiReader *arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;

    arg0->data = arg1;
    arg0->unk10 = arg2;
    arg0->unk1A = 0;
    arg0->unkC = 0;
    arg0->cursor = arg1;
    if (__alSeqReadWord((s32) arg0) == 0x4D546864) {
        __alSeqReadWord((s32) arg0);
        if (((__alSeqReadShort(arg0) << 0x10) == 0) && (__alSeqReadShort(arg0) == 1)) {
            temp_v0 = __alSeqReadShort(arg0);
            arg0->division = temp_v0;
            if (!(temp_v0 & 0x8000)) {
                arg0->tickScale = (f32) (1.0f / (f32) temp_v0);
                if (__alSeqReadWord((s32) arg0) == 0x4D54726B) {
                    __alSeqReadWord((s32) arg0);
                    arg0->trackStart = arg0->cursor;
                }
            }
        }
    }
}
