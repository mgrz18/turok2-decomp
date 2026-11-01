/* NON-MATCHING: 92 of 208 words differ (W batch). */
/* NON-MATCHING: 92 of 208 words differ, nearly all a one-word shift: the ROM copies hit->damage through a temporary
 * (lw v0 / addu a3,v0) before the MIN against the slot health, and allocates the shift constant and hit pointer to
 * other temporaries (t0/t1); the switch on the group uses beql in the ROM. Logic and layout are otherwise identical. */
#include "common.h"

/* Module hit callback (the level's onHit, func_00408FF8 + 8 at the link address): finds the first active part the
 * hit touched; parts 10-14 are three groups, each draining the health of one still-alive slot of the current phase.
 * A slot that dies changes the anim state and clears its bit; when no slot of the phase is left, the next phase
 * state starts. */

typedef struct {
    /* 0x00 */ s32 health;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ void *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 aliveMask;
    /* 0x668 */ u8 pad668[0x670 - 0x668];
    /* 0x670 */ s32 partsLeft;
    /* 0x674 */ s32 totalHealth;
    /* 0x678 */ s32 health;
} State;

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 mask;
} Part;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ Part *part;
} Hit;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 flags;
} CAI;

extern State D_8012FB70;
extern u8 D_00431D74[];
extern void func_0040BDD0(void *obj, void *sound, s32 a2);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_00442000(void *obj, CAI *ai, Hit *hit) {
    State *state;
    Part *part;
    s32 mask;
    s32 idx;
    s32 i;
    s32 group;
    s32 sound;
    s32 slot;
    s32 bit;

    part = hit->part;
    if (part == NULL) {
        return;
    }
    state = &D_8012FB70;
    i = 0;
    idx = -1;
    mask = state->activeMask & part->mask;
    for (; i < 15; i++) {
        if (idx >= 0) {
            break;
        }
        if ((1 << i) & mask) {
            idx = i;
        }
    }
    switch (idx) {
        case 10:
        case 11:
            group = 0;
            sound = 0xC00;
            break;
        case 12:
        case 14:
            group = 1;
            sound = 0x5000;
            break;
        case 13:
            group = 2;
            sound = 0x2000;
            break;
        default:
            return;
    }
    slot = -1;
    switch (state->phase) {
        case 0:
            if (group == 0) {
                if (state->slots[2].health > 0) {
                    slot = 2;
                } else if (state->slots[3].health > 0) {
                    slot = 3;
                } else if (state->slots[4].health > 0) {
                    slot = 4;
                }
            } else if (group == 1) {
                if (state->slots[6].health > 0) {
                    slot = 6;
                } else if (state->slots[7].health > 0) {
                    slot = 7;
                } else if (state->slots[8].health > 0) {
                    slot = 8;
                }
            }
            break;
        case 1:
            if (group == 0) {
                if (state->slots[1].health > 0) {
                    slot = 1;
                }
            } else if (group == 1) {
                if (state->slots[5].health > 0) {
                    slot = 5;
                }
            }
            break;
        case 2:
            if (group == 2) {
                if (state->slots[13].health > 0) {
                    slot = 13;
                }
            }
            break;
    }
    if (slot == -1) {
        return;
    }
    idx = hit->damage;
    if (state->slots[slot].health < idx) {
        idx = state->slots[slot].health;
    }
    state->slots[slot].health -= idx;
    state->health -= idx;
    func_0040BDD0(obj, D_00431D74, sound);
    if (state->slots[slot].health > 0) {
        return;
    }
    if (state->phase == 0) {
        state->partsLeft--;
        switch (group) {
            case 0:
                func_00243414(obj, ai, 7);
                break;
            case 1:
                func_00243414(obj, ai, 8);
                break;
        }
    }
    bit = 1 << slot;
    if (state->phase != 2) {
        ai->flags &= ~bit;
    }
    state->aliveMask &= ~bit;
    if (state->aliveMask == 0) {
        switch (state->phase) {
            case 0:
                func_00243414(obj, ai, 9);
                break;
            case 1:
                func_00243414(obj, ai, 0x13);
                break;
            case 2:
                func_00243414(obj, ai, 0x1E);
                break;
        }
    }
}
