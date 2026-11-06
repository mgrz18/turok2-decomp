/* FAKEMATCH: a dead per-mode switch before the remap test, and opt pinned to $s1 (see below). */
/* Builds the per-frame player input from the controller: optional button
 * remapping, then turn/look/strafe/move axes per control mode (Rage Wars has the
 * same code: func_802631B0_de in AbuN64/ragewars-decomp src/FX/802631D0.CPP).
 *
 * The ROM loads opt->m_ControlType once more than the remap test needs
 * (`lb $3, 0x2B($17)`, dead, before the live one). A switch on the mode whose
 * result the remap test overwrites leaves that load: its branches only go in
 * the delay-slot pass, after scheduling. It has to be a switch (beqz/beq on
 * the loaded value, so the load sits in $v1 beside `li $v0, 1` and right
 * before the deleted branch); a dead `if (mode < 2)` or a second call of the
 * remap test puts it in the wrong slot, and an empty if or a discarded call
 * leaves nothing. The ROM also keeps opt in a saved register though nothing
 * after the calls reads it; unpinned, opt lands in a temporary and every
 * access to it differs. */
#include "common.h"
#include "structs/player.h"
#include "globals/engine.h"

typedef struct {
    /* 0x00 */ u32 mask;
    /* 0x04 */ u32 bits;
} ButtonMap;

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ u32 buttons[5];
    /* 0xC4 */ u8 padC4[2];
    /* 0xC6 */ s8 stickX;
    /* 0xC7 */ s8 stickY;
} Controller;

typedef struct {
    /* 0x00 */ f32 turn;
    /* 0x04 */ f32 look;
    /* 0x08 */ f32 strafe;
    /* 0x0C */ f32 move;
    /* 0x10 */ Controller *ctrl;
    /* 0x14 */ f32 outTurn;
    /* 0x18 */ f32 outLook;
    /* 0x1C */ f32 outStrafe;
    /* 0x20 */ f32 outMove;
    /* 0x24 */ u32 buttons[5];
} PlayerInput;

extern ButtonMap sControlSide1ButtonMap[];
f32 Math_DecayToZero(f32 value, f32 rate);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* The remap table applies to control modes 0 and 1 with remapping on. */
void CPlayer__ReadInput(PlayerInput *in, CPlayer *pl) {
    register CPlayerOptions *opt asm("$17");
    Controller *ctrl;
    ButtonMap *map;
    f32 damp;
    f32 rate;
    f32 d;
    f32 x;
    u32 b;
    u32 mask;
    u32 bits;

    opt = pl->m_pOptions;
    damp = 1.0f - pl->m_ZoomBlend * 0.99f;
    rate = opt->m_SettingY * (1.0f / 128) - 1.0f;
    rate = rate * 0.75f + 1.0f;
    rate = rate * (gFrameIncrement * (damp * 0.1f));
    ctrl = in->ctrl;
    switch (opt->m_ControlType) {
    case 0:
        break;
    case 1:
        map = NULL;
        break;
    }
    map = NULL;
    if (opt->m_ControlType < 2) {
        if (opt->m_ControlType >= 0) {
            if (opt->m_ControlSide == 1) {
                map = sControlSide1ButtonMap;
            }
        }
    }
    if (map != NULL) {
        in->buttons[0] = 0;
        in->buttons[1] = 0;
        in->buttons[2] = 0;
        in->buttons[3] = 0;
        in->buttons[4] = 0;
        while (map->mask != 0) {
            mask = map->mask;
            bits = map->bits;
            if (ctrl->buttons[0] & mask) in->buttons[0] |= bits;
            if (ctrl->buttons[1] & mask) in->buttons[1] |= bits;
            if (ctrl->buttons[2] & mask) in->buttons[2] |= bits;
            if (ctrl->buttons[3] & mask) in->buttons[3] |= bits;
            if (ctrl->buttons[4] & mask) in->buttons[4] |= bits;
            map++;
        }
    } else {
        in->buttons[0] = ctrl->buttons[0];
        in->buttons[1] = ctrl->buttons[1];
        in->buttons[2] = ctrl->buttons[2];
        in->buttons[3] = ctrl->buttons[3];
        in->buttons[4] = ctrl->buttons[4];
    }

    switch (opt->m_ControlType) {
    case 0:
        b = in->buttons[0];
        if (b & 1) {
            in->strafe = 1.0f;
        } else if (b & 2) {
            in->strafe = -1.0f;
        } else {
            in->strafe = 0.0f;
        }
        in->turn = ctrl->stickX * 0.0125f;
        b = in->buttons[0];
        if (b & 8) {
            in->move = 1.0f;
        } else if (b & 4) {
            in->move = -1.0f;
        } else {
            in->move = 0.0f;
        }
        if (opt->m_bLookSpring) {
            in->look = ctrl->stickY * 0.0125f;
            break;
        }
        if (pl->m_bHeadMovementDisabled != 0) {
            break;
        }
        if (ctrl->stickY != 0) {
            in->look += rate * 0.0125f * ctrl->stickY;
        } else if (in->move != 0.0f && pl->m_SniperMode == 0) {
            d = in->move * (damp * 0.1f);
            if (d < 0.0f) {
                d = -d;
            }
            in->look = Math_DecayToZero(in->look, d);
        }
        if (in->look > 1.0f) {
            in->look = 1.0f;
        } else if (in->look < -1.0f) {
            in->look = -1.0f;
        }
        break;
    case 1:
        in->turn = ctrl->stickX * 0.0125f;
        b = in->buttons[0];
        if (b & 1) {
            in->strafe = 1.0f;
        } else if (b & 2) {
            in->strafe = -1.0f;
        } else {
            in->strafe = 0.0f;
        }
        if (pl->m_bHeadMovementDisabled == 0) {
            in->move = ctrl->stickY * 0.0125f;
        } else {
            in->move = 0.0f;
        }
        b = in->buttons[0];
        if (b & 0x10) {
            break;
        }
        if (b & 8) {
            x = in->look + rate * 0.75f;
            if (x > 1.0f) {
                x = 1.0f;
            }
            in->look = x;
        } else if (b & 4) {
            x = in->look - rate * 0.75f;
            if (x < -1.0f) {
                x = -1.0f;
            }
            in->look = x;
        } else if (opt->m_bLookSpring && in->move != 0.0f && pl->m_SniperMode == 0) {
            d = in->move * (damp * 0.1f);
            if (d < 0.0f) {
                d = -d;
            }
            in->look = Math_DecayToZero(in->look, d);
        }
        break;
    }
    in->outStrafe = in->strafe * ABS(in->strafe * in->strafe);
    if (in->turn * in->turn < 0.0f) {
        in->outTurn = in->turn * -(in->turn * in->turn);
    } else {
        in->outTurn = in->turn * (in->turn * in->turn);
    }
    in->outMove = in->move;
    in->outLook = in->look;
}
