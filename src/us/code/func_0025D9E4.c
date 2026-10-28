#include "common.h"

/* Picks the animation for the AI's current location: one candidate from the
 * location's primary flags and one from its secondary flags, each from the
 * table for the current mode; with both, a coin toss decides. */

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 anim;
} AnimEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ u16 flagsA;
    /* 0x18 */ u16 flagsB;
} Location;

typedef struct {
    /* 0x000 */ u8 pad0[0x94];
    /* 0x094 */ s8 location;
    /* 0x095 */ u8 pad95[0x133 - 0x95];
    /* 0x133 */ s8 modeIndex;
} CAI;

extern AnimEntry D_800AF6F0[];
extern AnimEntry D_800AF720[];
extern AnimEntry D_800AF750[];
extern AnimEntry D_800AF780[];
extern AnimEntry D_800AF7B0[];
extern AnimEntry D_800AF7E0[];

extern Location *func_0028479C(s8 *location);
extern s32 func_00245A98(void *actor, CAI *ai, AnimEntry *table, s32 flags);
extern s32 func_002113A4(void);

s32 func_0025D9E4(void *actor, CAI *ai) {
    Location *loc = func_0028479C(&ai->location);
    s32 animA = -1;
    s32 mode = ai->modeIndex;
    s32 animB = -1;

    if (loc->flagsA != 0) {
        switch (mode) {
        case 0:
            animA = func_00245A98(actor, ai, D_800AF6F0, loc->flagsA);
            break;
        case 1:
            animA = func_00245A98(actor, ai, D_800AF720, loc->flagsA);
            break;
        case 2:
            animA = func_00245A98(actor, ai, D_800AF750, loc->flagsA);
            break;
        }
    }
    if (loc->flagsB != 0) {
        switch (mode) {
        case 0:
            animB = func_00245A98(actor, ai, D_800AF780, loc->flagsB);
            break;
        case 1:
            animB = func_00245A98(actor, ai, D_800AF7B0, loc->flagsB);
            break;
        case 2:
            animB = func_00245A98(actor, ai, D_800AF7E0, loc->flagsB);
            break;
        }
    }
    if (animA != -1 && animB != -1) {
        if (func_002113A4() % 1000 >= 501) {
            return animA;
        }
        return animB;
    }
    if (animA != -1) {
        return animA;
    }
    if (animB != -1) {
        return animB;
    }
    return 0;
}
