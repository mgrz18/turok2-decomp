#include "common.h"

extern void *alSavePull(void *, s16 *, s32, s32, void *); /* alSavePull */
extern s32 alSaveParam(s32, s32, s32);
extern void alFilterNew(void *, void *, void *, s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} SaveFilter;

void alSaveNew(SaveFilter *arg0) {
    alFilterNew(arg0, alSavePull, alSaveParam, 3);
    arg0->unk14 = 0;
    arg0->unk18 = 1;
}
