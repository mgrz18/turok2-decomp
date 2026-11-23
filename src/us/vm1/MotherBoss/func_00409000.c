/* 0x00409000-0x00409340. splat started it at 0x00408FF8, two junk words before
 * it (src/us/asm/stray_00408FF8.s); MotherBoss_Init installs func_00408FF8 + 8,
 * this function, as the AI hit callback. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "globals/boss.h"
#include "protos/ai.h"

/* Boss hit callback: finds which weak spot the hit part covers, takes the
 * damage off the matching live spot for the current phase, and when a spot
 * dies plays the stagger animation and clears its bit; when all spots of the
 * phase are gone, moves the boss to its next state. */


extern void MotherBoss_StartSpotAnims(void *obj, void *sounds, s32 flags);

void MotherBoss_Hurt(void *obj, CAI *ai, DamageEvent *hit) {
    MotherBossState *s;
    s32 mask;
    s32 spot;
    s32 i;
    s32 kind;
    s32 fx;
    s32 target;
    s32 damage;
    s32 bit;

#ifdef TARGET_PORT
    {
        extern void T2Port_LogBossHurt(const char *who, void *obj, void *hit); /* DEBUG-ONLY */
        T2Port_LogBossHurt("mother", obj, hit);
    }
#endif
    if (hit->frame == NULL) {
        return;
    }
    i = 0;
    spot = -1;
    s = &gMotherBoss;
    mask = s->activeMask & hit->frame->unk_6C;
    for (; i < 15; i++) {
        if (spot >= 0) {
            break;
        }
        bit = 1 << i;
        if (bit & mask) {
            spot = i;
        }
    }
    switch (spot) {
    case 10:
    case 11:
        kind = 0;
        fx = 0xC00;
        break;
    case 12:
    case 14:
        kind = 1;
        fx = 0x5000;
        break;
    case 13:
        kind = 2;
        fx = 0x2000;
        break;
    default:
        return;
    }
    target = -1;
    switch (s->phase) {
    case 0:
        if (kind == 0) {
            if (s->slots[2].health > 0) {
                target = 2;
            } else if (s->slots[3].health > 0) {
                target = 3;
            } else if (s->slots[4].health > 0) {
                target = 4;
            }
        } else if (kind == 1) {
            if (s->slots[6].health > 0) {
                target = 6;
            } else if (s->slots[7].health > 0) {
                target = 7;
            } else if (s->slots[8].health > 0) {
                target = 8;
            }
        }
        break;
    case 1:
        if (kind == 0) {
            if (s->slots[1].health > 0) {
                target = 1;
            }
        } else if (kind == 1) {
            if (s->slots[5].health > 0) {
                target = 5;
            }
        }
        break;
    case 2:
        if (kind == 2) {
            if (s->slots[13].health > 0) {
                target = 13;
            }
        }
        break;
    }
    if (target == -1) {
        return;
    }
    damage = s->slots[target].health < hit->damage ? (s->slots + target)->health : hit->damage;
    s->slots[target].health -= damage;
    s->health -= damage;
    MotherBoss_StartSpotAnims(obj, sMotherBossHurtScript, fx);
    if (s->slots[target].health > 0) {
        return;
    }
    if (s->phase == 0) {
        s->partsLeft--;
        switch (kind) {
        case 0:
            CAI__SetMode(obj, ai, 7);
            break;
        case 1:
            CAI__SetMode(obj, ai, 8);
            break;
        }
    }
    bit = 1 << target;
    if (s->phase != 2) {
        ai->m_VisBits &= ~bit;
    }
    s->liveMask &= ~bit;
    if (s->liveMask != 0) {
        return;
    }
    switch (s->phase) {
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
