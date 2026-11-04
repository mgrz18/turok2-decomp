#include "common.h"
#include "structs/player.h"

typedef struct Def {
    s32 kind;              /* 0x00 */
} Def;

typedef struct Actor {
    char pad0[0x14];
    Def *def;              /* 0x014 */
    char pad18[0xB8 - 0x18];
    u16 type;              /* 0x0B8 */
    char padBA[0xD4 - 0xBA];
    s32 flags;             /* 0x0D4 */
    char padD8[0x1A8 - 0xD8];
    CPlayer *owner;         /* 0x1A8 */
    char pad1AC[0x27C - 0x1AC];
    s32 unk27C;            /* 0x27C */
} Actor;

typedef struct Target {
    char pad0[4];
    s32 unk4;              /* 0x04 */
    char pad8[0x68 - 0x8];
    Actor *unk68;          /* 0x68 */
    char pad6C[0x94 - 0x6C];
    s8 unk94;              /* 0x94 */
} Target;

extern s32 D_8011001C;

/* WRONG CUT: the ROM splits this function at 0x245E20 (func_00245E20 = its last 10 words,
   reached by branches from here). Compiled whole it is 0xF8 bytes; the first 0xD0 match
   func_00245D50 and the tail matches func_00245E20 word for word. */

s32 func_00245D50(Actor *self, Target *target, Actor *actor) {
    CPlayer *owner;

    if (target->unk4 == 0) {
        return 6;
    }
    if (actor == NULL) {
        if (target->unk94 != 0) {
            return 4;
        }
        return 3;
    }
    if (actor == target->unk68) {
        return 2;
    }
    if (actor->def->kind == 5) {
        return 5;
    }
    if (actor->type == 0x64F) {
        return 7;
    }
    if (D_8011001C == 0) {
        if (actor->flags & 0x300000) {
            owner = actor->owner;
            if (0.0f < owner->m_FleeTime) {
                if (self->type != 0xCA || owner->m_Data.m_CurrentWeapon != 0x14) {
                    return 1;
                }
            }
            if (owner->m_pCrossHairTarget == self && owner->m_CrossHairMode == 2) {
                return 1;
            }
        }
        if ((self->unk27C & 2) && self->type != 0xCA) {
            return 1;
        }
    }
    return 0;
}
