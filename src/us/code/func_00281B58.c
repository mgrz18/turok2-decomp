#include "common.h"
#include "structs/camera.h"

/* Resets a camera text (see func_002821AC.c): two words cleared. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 unk1C;
} TextReset;

void func_00281B58(TextReset *arg0) {
    arg0->unk14 = 0;
    arg0->unk1C = 0;
}
