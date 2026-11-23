#include "common.h"

/* Only what this reads and writes. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 value;
    /* 0x08 */ u8 pad8[0xB4 - 0x8];
    /* 0xB4 */ s32 repeated;
} Tracker;

/* Sets the value, noting when it is the same as before. */
void ValueTracker_Set(Tracker *arg0, s32 arg1) {
    if (arg0->value == arg1) {
        arg0->repeated = 1;
    }
    arg0->value = arg1;
}
