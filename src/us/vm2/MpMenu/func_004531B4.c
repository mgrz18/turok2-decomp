#include "common.h"
#include "structs/controller.h"
#include "globals/game.h"
#include "globals/rumble.h"
#include "globals/menu.h"
#include "protos/game.h"


/* Controller / rumble slot, 0x224 bytes (see Cont_ResetSlot). */

typedef struct Layer {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ s32 flagsA;
    /* 0x84 */ u8 pad84[0xA8 - 0x84];
    /* 0xA8 */ s32 flagsB;
} Layer;

typedef struct Screen {
    /* 0x0 */ s16 state;
    /* 0x2 */ u8 pad2[0xC - 2];
    /* 0xC */ Layer *layer;
} Screen;


void RumblePool_FreeAll(RumblePool *pool);
void Controller_ResetAllRumble(void);
void LoopSounds_Pause(void *);

void PauseMenu_Init_2(Screen *screen) {
    s32 i;

    screen->state = 2;
    for (i = 0; i < 4; i++) {
        sPauseRumbleOn[i] = gControllers[i].rumbleOn;
    }
    RumblePool_FreeAll(&gRumblePool);
    Controller_ResetAllRumble();
    if (gEngineApp_m_Settings.multiplayer) {
        screen->layer->flagsA &= ~0x800000;
        screen->layer->flagsB &= ~0x800000;
    } else {
        screen->layer->flagsA |= 0x800000;
        screen->layer->flagsB |= 0x800000;
    }
    LoopSounds_Pause(LoopSounds_Get());
}
