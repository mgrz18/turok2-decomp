#ifndef STRUCTS_CONTROLLER_H
#define STRUCTS_CONTROLLER_H

/* A controller's state, gControllers[4] (one per port, 0x224 bytes each;
 * gMergedController is a fifth one that merges the four). Rage Wars has the same
 * object (its D_8010F328[], named ControllerProfile in rw_abu's
 * include/COMMON/ITEMRECAT20V1.H); CPlayer::m_ContState.m_pContState
 * (CPlayer + 0xA50) points to the player's, and a menu window reads one
 * (Menu::controller, structs/menu.h).
 *
 * Controller_ProcessInput reads a port into the input* fields (the stick at 0xC4,
 * the buttons from 0x14); once a frame Controller_Latch copies them into the
 * fields from 0xB0 and 0xC6, which is what the game reads. The button
 * groups are the ones CPlayerContState (structs/player.h) has: held,
 * just pressed ("single"), let go quickly ("single, no hold"), pressed
 * twice ("double") and auto-repeat (the menus move their cursor with it).
 * This header needs only common.h: the libultra OSPfs at 0xD8 is a byte
 * array here (rw/abu_types.h and ultra_types.h each have their own OSPfs). */

#ifndef RW_ABU_TYPES_H /* it has the base types too */
#include "common.h"
#include "ultra_types.h"
#endif

/* One button's counters (Controller_ProcessInput). */
typedef struct ControllerButtonHistory {
    /* 0x0 */ s8 taps;              /* presses less than 30 frames apart */
    /* 0x1 */ s8 age;               /* frames since the last press, up to 30 */
    /* 0x2 */ s8 repeat;            /* auto-repeat countdown (0x30 first, then 4) */
    /* 0x3 */ s8 held;              /* frames held, up to 255 */
} ControllerButtonHistory; /* size 0x4 */

typedef struct ControllerProfile {
    /* 0x000 */ s32 connected;      /* a controller is plugged in */
    /* 0x004 */ s8 port;
    /* 0x005 */ u8 _pad5[3];
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u32 inputPreviousButtons;
    /* 0x018 */ u32 inputButtons;
    /* 0x01C */ u32 inputButtonsSingle;
    /* 0x020 */ u32 inputButtonsSingleNoHold;
    /* 0x024 */ u32 inputButtonsDouble;
    /* 0x028 */ u32 inputButtonsRepeat;
    /* 0x02C */ ControllerButtonHistory buttonHistory[32]; /* one per button bit */
    /* 0x0AC */ u32 previousButtons;
    /* 0x0B0 */ u32 buttons;        /* held (A 0x8000, B 0x4000, START 0x1000; the stick as 0xF0000) */
    /* 0x0B4 */ u32 buttonsSingle;  /* just pressed */
    /* 0x0B8 */ u32 buttonsSingleNoHold;
    /* 0x0BC */ u32 buttonsDouble;
    /* 0x0C0 */ u32 buttonsRepeat;
    /* 0x0C4 */ s8 inputStickX;
    /* 0x0C5 */ s8 inputStickY;
    /* 0x0C6 */ s8 stickX;          /* -80..80 */
    /* 0x0C7 */ s8 stickY;
    /* 0x0C8 */ s32 rumblePak;      /* a Rumble Pak answered osMotorInit */
    /* 0x0CC */ s32 rumbleOn;       /* the rumble option (kept in sPauseRumbleOn) */
    /* 0x0D0 */ f32 rumbleLevel;
    /* 0x0D4 */ f32 rumbleTimer;
    /* 0x0D8 */ u8 rumblePfs[0x68]; /* the OSPfs osMotorInit fills */
    /* 0x140 */ u8 rumblePool[0x28]; /* the rumble effects' free and active lists (RumblePool_Init) */
    /* 0x168 */ f32 rumbleBase;     /* the level the effects add to (the weapon spin...) */
    /* 0x16C */ u8 rumbleNodes[3][0x3C]; /* the pool's 3 effects */
    /* 0x220 */ s32 reset;
} ControllerProfile; /* size 0x224 */

extern ControllerProfile gControllers[4];
extern ControllerProfile gMergedController;   /* the four merged */
extern OSContStatus gContStatus[4];   /* what osContInit / osContGetQuery report per port (errno bit 3: nothing plugged in) */

#endif
