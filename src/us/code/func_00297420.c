#include "common.h"

/* records: base of an array of 0x4C-byte voice records, each with its
 * filter at 0x20. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 bus;
    /* 0x34 */ s32 records;
} Synth;

s32 alFxNew(s32, s32, s32);           /* extern */
s32 alMainBusParam(s32, s32, s32);           /* extern */
s32 alFxParam(s32, s32, s32);           /* extern */

s32 alSynAllocFX(Synth *arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp_a2;
    s32 temp_s0;

    temp_s0 = arg1 * 0x4C;
    alFxNew(arg0->records + temp_s0 + 0x20, arg2, arg3);
    temp_a2 = arg0->records + temp_s0;
    alFxParam(temp_a2 + 0x20, 1, temp_a2);
    alMainBusParam(arg0->bus, 2, arg0->records + temp_s0 + 0x20);
    return arg0->records + temp_s0 + 0x20;
}
