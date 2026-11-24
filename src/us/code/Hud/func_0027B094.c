#include "common.h"

/* The font colour COnScreen__SetFontColor sets (gEngineApp_m_OnScreen_m_TopR): six words, in argument order. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} FontColor;

#include "globals/onscreen.h"
/* m_TopR and the five words after it, as one view (the field's real type changes the ROM's code here). */
#define sFontColor (*(FontColor *) &gEngineApp_m_OnScreen_m_TopR)

void COnScreen__SetFontColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    sFontColor.unk0 = arg0;
    sFontColor.unk4 = arg1;
    sFontColor.unk8 = arg2;
    sFontColor.unkC = arg3;
    sFontColor.unk10 = arg4;
    sFontColor.unk14 = arg5;
}
