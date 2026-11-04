#include "common.h"

s32 func_00293354(s32);

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

s16 func_00293324(void *);                          /* extern */
extern f32 D_800AA420;

void func_00292F48(MidiReader *arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;

    arg0->data = arg1;
    arg0->unk10 = arg2;
    arg0->unk1A = 0;
    arg0->unkC = 0;
    arg0->cursor = arg1;
    if (func_00293354((s32) arg0) == 0x4D546864) {
        func_00293354((s32) arg0);
        if (((func_00293324(arg0) << 0x10) == 0) && (func_00293324(arg0) == 1)) {
            temp_v0 = func_00293324(arg0);
            arg0->division = temp_v0;
            if (!(temp_v0 & 0x8000)) {
                arg0->tickScale = (f32) (D_800AA420 / (f32) temp_v0);
                if (func_00293354((s32) arg0) == 0x4D54726B) {
                    func_00293354((s32) arg0);
                    arg0->trackStart = arg0->cursor;
                }
            }
        }
    }
}
