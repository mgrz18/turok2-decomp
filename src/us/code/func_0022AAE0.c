/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_2.CPP, func_8027B428_de, with Turok 2's layouts. */

#include "common.h"

typedef struct ActorDef {
    /* 0x00 */ s32 type;
} ActorDef;

typedef struct Owner {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1[0xD4 - 1];
    /* 0xD4 */ s32 flags;
} Owner;

typedef struct Particle {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ u8 pad4[0x14 - 4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x10C - 0x18];
    /* 0x10C */ Owner *owner;
} Particle;

typedef struct CollisionState {
    /* 0x00 */ u8 kind;
} CollisionState;

extern s32 D_800F1D7C;
extern CollisionState *D_800F1CE0;

void func_0026E19C(s32 state, s32 bits, Particle *p);
void func_0026E110(CollisionState *state, s32 bits, Particle *p);

static __inline__ s32 isGroupA(Particle *p) {
    switch (p->type) {
        case 2:
        case 0x56:
        case 0x111:
        case 0x126:
            return 1;
    }
    return 0;
}

static __inline__ s32 isGroupB(Particle *p) {
    switch (p->type) {
        case 5:
        case 6:
        case 7:
        case 9:
        case 0x11:
        case 0x13:
        case 0x1A:
        case 0x101:
        case 0x110:
            return 1;
    }
    return 0;
}

static __inline__ s32 isGroupC(Particle *p) {
    switch (p->type) {
        case 8:
        case 0xD:
        case 0x12:
            return 1;
    }
    return 0;
}

/* Reports a particle to the two collision states by the type group it belongs to: with
 * one set of bits when its owner is a kind 1 object flagged 0x300000, otherwise with
 * another when its descriptor is of kind 1 or 4. */
void func_0022AAE0(Particle *p) {
    if (D_800F1D7C != 0) {
        Owner *owner = p->owner;

        if (owner != NULL && owner->kind == 1 && (owner->flags & 0x300000)) {
            if (isGroupA(p)) {
                func_0026E19C(D_800F1D7C, 0x10, p);
            }
            if (isGroupB(p)) {
                func_0026E19C(D_800F1D7C, 8, p);
            }
            if (isGroupC(p)) {
                func_0026E19C(D_800F1D7C, 0x20, p);
            }
        } else if (p->def->type == 1 || p->def->type == 4) {
            if (isGroupC(p)) {
                func_0026E19C(D_800F1D7C, 0x200, p);
            }
        }
    }
    if (D_800F1CE0 != NULL && D_800F1CE0->kind == 1) {
        Owner *owner = p->owner;

        if (owner != NULL && owner->kind == 1 && (owner->flags & 0x300000)) {
            if (isGroupA(p)) {
                func_0026E110(D_800F1CE0, 8, p);
            }
            if (isGroupB(p)) {
                func_0026E110(D_800F1CE0, 4, p);
            }
            if (isGroupC(p)) {
                func_0026E110(D_800F1CE0, 0x10, p);
            }
        } else if (p->def->type == 1 || p->def->type == 4) {
            if (isGroupA(p)) {
                func_0026E110(D_800F1CE0, 0x80, p);
            }
            if (isGroupB(p)) {
                func_0026E110(D_800F1CE0, 0x40, p);
            }
            if (isGroupC(p)) {
                func_0026E110(D_800F1CE0, 0x100, p);
            }
        }
    }
}
