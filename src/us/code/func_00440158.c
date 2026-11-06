#include "common.h"

/* D_8012FA24: two words reset to -1; the word just before it (D_8012FA20) holds a table pointer. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} State8012FA24;

extern s32 D_00431544;
extern s32 D_00431580;
extern s32 D_004315BC;
extern s32 D_00431620;
extern State8012FA24 D_8012FA24;

void Primagen_SetSoundScript_2(s32 arg0) {
    D_8012FA24.unk0 = -1;
    D_8012FA24.unk4 = -1;
    if (arg0 == 0) {
        ((s32 **) &D_8012FA24)[-1] = NULL;
    }
    if (arg0 == 1) {
        ((s32 **) &D_8012FA24)[-1] = &D_00431580;
    }
    if (arg0 == 4) {
        ((s32 **) &D_8012FA24)[-1] = &D_00431620;
    }
    if (arg0 == 2) {
        ((s32 **) &D_8012FA24)[-1] = &D_00431544;
    }
    if (arg0 == 3) {
        ((s32 **) &D_8012FA24)[-1] = &D_004315BC;
    }
}
