#include "common.h"

/* Only the field written here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
} Voice16;

void alSynSetPriority(s32 arg0, Voice16 *arg1, s16 arg2) {
    arg1->unk16 = arg2;
}
