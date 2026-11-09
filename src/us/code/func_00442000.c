#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "globals/boss.h"
#include "protos/ai.h"

/* Module hit callback (the level's onHit, func_00408FF8 + 8 at the link address): finds the first active part the
 * hit touched; parts 10-14 are three groups, each draining the health of one still-alive slot of the current phase.
 * A slot that dies changes the anim state and clears its bit; when no slot of the phase is left, the next phase
 * state starts. */

extern void MotherBoss_StartSpotAnims_2(void *obj, void *sound, s32 a2);

void MotherBoss_Hurt_2(void *obj, CAI *ai, DamageEvent *hit) {
    MotherBossState *state;
    HitFrame *part;
    s32 mask;
    s32 idx;
    s32 i;
    s32 group;
    s32 sound;
    s32 slot;
    s32 bit;

    part = hit->frame;
    if (part == NULL) {
        return;
    }
    state = &gMotherBoss;
    i = 0;
    idx = -1;
    mask = state->activeMask & part->unk_6C;
    for (; i < 15; i++) {
        if (idx >= 0) {
            break;
        }
        bit = 1 << i;
        if (bit & mask) {
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
    /* the damage dealt, capped at the slot health */
    idx = state->slots[slot].health >= hit->damage ? hit->damage : state->slots[slot].health;
    state->slots[slot].health -= idx;
    state->health -= idx;
    MotherBoss_StartSpotAnims_2(obj, sMotherBossHurtScript_2, sound);
    if (state->slots[slot].health > 0) {
        return;
    }
    if (state->phase == 0) {
        state->partsLeft--;
        switch (group) {
            case 0:
                CAI__SetMode(obj, ai, 7);
                break;
            case 1:
                CAI__SetMode(obj, ai, 8);
                break;
        }
    }
    bit = 1 << slot;
    if (state->phase != 2) {
        ai->m_VisBits &= ~bit;
    }
    state->liveMask &= ~bit;
    if (state->liveMask == 0) {
        switch (state->phase) {
            case 0:
                CAI__SetMode(obj, ai, 9);
                break;
            case 1:
                CAI__SetMode(obj, ai, 0x13);
                break;
            case 2:
                CAI__SetMode(obj, ai, 0x1E);
                break;
        }
    }
}
