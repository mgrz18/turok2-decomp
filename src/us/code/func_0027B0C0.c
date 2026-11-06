#include "common.h"

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
} FontScale;

extern FontScale gEngineApp_m_OnScreen_m_FontXScale;

void COnScreen__SetFontScale(f32 arg0, f32 arg1) {
    gEngineApp_m_OnScreen_m_FontXScale.x = arg0;
    gEngineApp_m_OnScreen_m_FontXScale.y = arg1;
}
