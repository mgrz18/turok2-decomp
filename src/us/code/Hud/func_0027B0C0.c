#include "common.h"

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
} FontScale;

#include "globals/onscreen.h"
/* m_FontXScale and the word after it, as one view (the field's real type changes the ROM's code here). */
#define sFontScale (*(FontScale *) &gEngineApp_m_OnScreen_m_FontXScale)

void COnScreen__SetFontScale(f32 arg0, f32 arg1) {
    sFontScale.x = arg0;
    sFontScale.y = arg1;
}
