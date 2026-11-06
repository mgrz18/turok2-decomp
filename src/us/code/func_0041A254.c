#include "common.h"
#include "structs/controller.h"
#include "globals/game.h"
#include "globals/rumble.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ u32 flags80;
    /* 0x84 */ u8 pad84[0x24];
    /* 0xA8 */ u32 flagsA8;
} A254Screen;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ A254Screen *screen;
} A254Menu;


extern s32 sPauseRumbleOn[4];
extern void Controller_ResetAllRumble(void);
extern void RumblePool_FreeAll(RumblePool *pool);
extern s32 LoopSounds_Get(void);
extern void LoopSounds_Pause(s32);

void PauseMenu_Init(A254Menu *menu) {
    s32 i;

    menu->state = 2;
    for (i = 0; i < 4; i++) {
        sPauseRumbleOn[i] = gControllers[i].rumbleOn;
    }
    RumblePool_FreeAll(&gRumblePool);
    Controller_ResetAllRumble();
    if (gEngineApp_m_Settings.multiplayer) {
        menu->screen->flags80 &= ~0x800000;
        menu->screen->flagsA8 &= ~0x800000;
    } else {
        menu->screen->flags80 |= 0x800000;
        menu->screen->flagsA8 |= 0x800000;
    }
    LoopSounds_Pause(LoopSounds_Get());
}
