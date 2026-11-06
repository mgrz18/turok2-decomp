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

extern FontColor gEngineApp_m_OnScreen_m_TopR;

void COnScreen__SetFontColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    gEngineApp_m_OnScreen_m_TopR.unk0 = arg0;
    gEngineApp_m_OnScreen_m_TopR.unk4 = arg1;
    gEngineApp_m_OnScreen_m_TopR.unk8 = arg2;
    gEngineApp_m_OnScreen_m_TopR.unkC = arg3;
    gEngineApp_m_OnScreen_m_TopR.unk10 = arg4;
    gEngineApp_m_OnScreen_m_TopR.unk14 = arg5;
}
