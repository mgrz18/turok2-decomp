#include "common.h"

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
} FontScale;

extern FontScale gEngineApp_m_OnScreen_m_FontXScale; /* local type: the field's real type (or the member path) changes the ROM's code here */

void COnScreen__SetFontScale(f32 arg0, f32 arg1) {
    gEngineApp_m_OnScreen_m_FontXScale.x = arg0;
    gEngineApp_m_OnScreen_m_FontXScale.y = arg1;
}
