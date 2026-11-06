#include "common.h"

/* What this sets on its first argument: a value and a mode. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 value;
    /* 0x8 */ s32 mode;
} ModeState;

void COverlay__Update();                               /* extern */

void COverlay__StartScript(ModeState *arg0, s32 arg1) {
    arg0->value = arg1;
    arg0->mode = 2;
    COverlay__Update();
}
