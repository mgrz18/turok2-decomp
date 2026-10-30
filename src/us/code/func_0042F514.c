#include "common.h"

/* Settings block, 0x184 bytes: a copy lives in the timer state and is
 * committed to D_8011AAD8. */
typedef struct Settings {
    s32 words[0x184 / 4];
} Settings;

/* The module's timer state, D_800F56B0 (see func_0042F1C4). */
typedef struct TimerState {
    /* 0x000 */ s32 active;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8 pad0C[0x1C8 - 0xC];
    /* 0x1C8 */ s32 pending;
    /* 0x1CC */ Settings settings;
} TimerState;

extern TimerState D_800F56B0;
extern Settings D_8011AAD8;

void func_0042F514(void) {
    TimerState *s = &D_800F56B0;

    if (s->pending != 0) {
        s->pending = 0;
        D_8011AAD8 = s->settings;
    }
    s->active = 0;
    s->unk4 = 0;
    s->unk8 = 0;
}
