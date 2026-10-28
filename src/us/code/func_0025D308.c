#include "common.h"

/* AI dodge check: if the definition enables dodging for the current mode,
 * dodges with a probability that grows with the AI's skill (0..255), to the
 * side away from the threat (animation 8 or 9). */

typedef struct {
    /* 0x00 */ u8 pad0[0x7C];
    /* 0x7C */ u16 dodgeMask; /* 5 bits per mode */
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x80];
    /* 0x080 */ s32 threat;
    /* 0x084 */ u8 pad84[0xC8 - 0x84];
    /* 0x0C8 */ u8 skill;
    /* 0x0C9 */ u8 padC9[0x133 - 0xC9];
    /* 0x133 */ s8 modeIndex;
} CAI;

extern s32 func_002113A4(void);
extern s32 func_002451AC(Actor *actor, CAI *ai, s32 threat, f32 angle);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);

void func_0025D308(Actor *actor, CAI *ai) {
    f32 skill;
    s32 chance;
    s32 side;
    s32 mode;
    s32 mode2;

    mode = ai->modeIndex;
    mode2 = mode;
    if ((mode == 0 && (actor->def->dodgeMask & 0x1F)) || (mode == 1 && (actor->def->dodgeMask & 0x3E0))
        || (mode2 == 2 && (actor->def->dodgeMask & 0x7C00))) {
        skill = ai->skill * (1.0f / 255);
        chance = skill * (skill * 600.0f) + 100.0f;
        if (func_002113A4() % 10000 < chance) {
            side = func_002451AC(actor, ai, ai->threat, 0.261799425f);
            if (side == -1) {
                func_00243414(actor, ai, 8);
            } else if (side == 1) {
                func_00243414(actor, ai, 9);
            }
        }
    }
}
