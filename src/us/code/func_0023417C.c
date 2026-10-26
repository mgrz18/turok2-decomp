/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A7164.c, func_002A7164, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Mtx44 { s32 w[16]; } Mtx44;

typedef struct EmitCfg { u8 pad0[0x14]; s32 unk14; } EmitCfg;
/* Turok 2 actor: matrices at 0x48, cfg at 0xF8. */
typedef struct EmitActor { u8 pad0[0x48]; Mtx44 mtx[2]; u8 padC8[0x30]; EmitCfg *cfg; } EmitActor;

typedef struct EmitInfo {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
} EmitInfo;

typedef struct EmitNode EmitNode;
struct EmitNode {
    /* 0x00 */ EmitNode *prev;
    /* 0x04 */ EmitNode *next;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ u8 rot[0xC];
    /* 0x28 */ Mtx44 mtx[2];
    /* 0xA8 */ f32 fraction;
    /* 0xAC */ f32 length;
    /* 0xB0 */ EmitActor *owner;
};

typedef struct Emitter {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ EmitInfo *info;
    /* 0x0C */ u8 padC[0x24 - 0xC];
    /* 0x24 */ f32 unk24;
    /* 0x28 */ u8 pad28[0x3C - 0x28];
    /* 0x3C */ u32 flags;
    /* 0x40 */ EmitNode *nodes;
    /* 0x44 */ u8 pad44[0x4C - 0x44];
    /* 0x4C */ f32 totalLength;
} Emitter;

extern s32 D_800B6D1C;
void func_0020D274(void *, void *);
void func_0020D908(void *, void *);
void func_0020EF2C(Vec3f *, Vec3f *, Vec3f *);
void func_0020F7EC(void *);
void func_002101A0(void *, void *);
void func_002101BC(void *, void *);
void func_0021034C(void *, f32, f32, f32);
f32 func_00298470(f32);                 /* sqrtf */

/* Rage Wars' func_802A6174: places one node on its owner, or freezes it when the owner went away (-1). */
static __inline__ void placeNode(Emitter *e, EmitNode *n, EmitActor *o) {
    u8 m[0x40];
    f32 k;

    if (o == (EmitActor *)-1) {
        n->mtx[D_800B6D1C] = n->mtx[D_800B6D1C ^ 1];
        n->owner = NULL;
    } else {
        func_0020D908(m, &o->mtx[D_800B6D1C]);
        k = 0.01f;
        if (o->cfg->unk14 != 0) {
            k = 0.29257143f;
        }
        func_0021034C(m, k, k, k);
        func_0020F7EC(m);
        func_002101A0(m, &n->pos);
        if (e->flags & 4) {
            func_002101BC(m, n->rot);
        } else {
            func_0020D274(m, &n->mtx[D_800B6D1C]);
        }
    }
}

/* Places every node of the emitter on its owner and sums the distances between consecutive nodes. */
void func_0023417C(Emitter *e) {
    Vec3f d;
    EmitNode *n;
    EmitActor *o;
    EmitNode *prev;
    f32 len;
    s32 moved;

    n = e->nodes;
    moved = 0;
    e->totalLength = 0.0f;
    if (n != NULL) {
        do {
            o = n->owner;
            if (o != NULL) {
                placeNode(e, n, o);
                moved = 1;
                n->unk8 = e->info->unk8;
            }
            prev = n->prev;
            if (prev != NULL) {
                func_0020EF2C(&d, &n->pos, &prev->pos);
                len = func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
                prev->length = len;
                e->totalLength += len;
            }
            n = n->next;
        } while (n != NULL);
    }
    if (moved) {
        e->unk24 = e->info->unk0;
    }
}
