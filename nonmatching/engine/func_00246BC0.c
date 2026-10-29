/* NON-MATCHING: 6 of 330 words differ: the ROM loads opt->controlMode twice in
 * a row before the `< 2` test (`lb $3, 0x2B($17)` x2, the first one dead) and
 * keeps opt in $s1 although nothing after the calls reads it. Both look like
 * leftovers of an inlined helper; with the helper below and opt pinned to $s1
 * this C differs only there (a copy `a0 = s1` for the helper parameter instead
 * of the dead load, plus the schedule around it). Without the pin opt lands in
 * a temporary register and nearly every word differs.
 *
 * Builds the per-frame player input from the controller: optional button
 * remapping, then turn/look/strafe/move axes per control mode (Rage Wars has the
 * same code: func_802631B0_de in AbuN64/ragewars-decomp src/FX/802631D0.CPP). */
#include "common.h"

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
    /* 0x00 */ u8 pad0[0x2A];
    /* 0x2A */ u8 sensitivity;
    /* 0x2B */ s8 controlMode;
    /* 0x2C */ u8 remap;
    /* 0x2D */ u8 stickLook;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[0xB8C - 0x51C];
    /* 0xB8C */ s32 unkB8C;
    /* 0xB90 */ f32 unkB90;
    /* 0xB94 */ u8 padB94[0x1284 - 0xB94];
    /* 0x1284 */ s32 unk1284;
} Player;

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

extern f32 D_800B6D28;
extern ButtonMap D_800ADA70[];
f32 func_0021170C(f32 value, f32 rate);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* The remap table applies to control modes 0 and 1 with remapping on. */
static __inline__ ButtonMap *GetButtonMap(s32 mode, PlayerOptions *o) {
    if (mode < 2) {
        if (mode >= 0) {
            if (o->remap == 1) {
                return D_800ADA70;
            }
        }
    }
    return NULL;
}

void func_00246BC0(PlayerInput *in, Player *pl) {
    register PlayerOptions *opt asm("$17");
    Controller *ctrl;
    ButtonMap *map;
    f32 damp;
    f32 rate;
    f32 d;
    f32 x;
    u32 b;
    u32 mask;
    u32 bits;

    opt = pl->options;
    damp = 1.0f - pl->unkB90 * 0.99f;
    rate = opt->sensitivity * (1.0f / 128) - 1.0f;
    rate = rate * 0.75f + 1.0f;
    rate = rate * (D_800B6D28 * (damp * 0.1f));
    ctrl = in->ctrl;
    map = GetButtonMap(opt->controlMode, opt);
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

    switch (opt->controlMode) {
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
        if (opt->stickLook) {
            in->look = ctrl->stickY * 0.0125f;
            break;
        }
        if (pl->unk1284 != 0) {
            break;
        }
        if (ctrl->stickY != 0) {
            in->look += rate * 0.0125f * ctrl->stickY;
        } else if (in->move != 0.0f && pl->unkB8C == 0) {
            d = in->move * (damp * 0.1f);
            if (d < 0.0f) {
                d = -d;
            }
            in->look = func_0021170C(in->look, d);
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
        if (pl->unk1284 == 0) {
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
        } else if (opt->stickLook && in->move != 0.0f && pl->unkB8C == 0) {
            d = in->move * (damp * 0.1f);
            if (d < 0.0f) {
                d = -d;
            }
            in->look = func_0021170C(in->look, d);
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
