/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A7164.c, func_002A7164, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "globals/engine.h"

typedef struct Mtx44 { s32 w[16]; } Mtx44;

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
    /* 0x10 */ CVector3 pos;
    /* 0x1C */ u8 rot[0xC];
    /* 0x28 */ Mtx44 mtx[2];
    /* 0xA8 */ f32 fraction;
    /* 0xAC */ f32 length;
    /* 0xB0 */ CParticle *owner;
};

/* structs/swoosh.h Swoosh */
typedef struct Emitter {
    /* 0x00 */ void *prev;
    /* 0x04 */ void *next;
    /* 0x08 */ EmitInfo *info;
    /* 0x0C */ void *tex0;
    /* 0x10 */ void *tex1;
    /* 0x14 */ s32 texture[2];
    /* 0x1C */ void *owner;
    /* 0x20 */ s32 id;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ u8 pad28[0x3C - 0x28];
    /* 0x3C */ u32 flags;
    /* 0x40 */ EmitNode *nodes;
    /* 0x44 */ EmitNode *tail;
    /* 0x48 */ s32 count;
    /* 0x4C */ f32 totalLength;
} Emitter;

void CMtxF__ToMtxAffine(void *, void *);
void CMtxF__FromMtxAffine(void *, void *);
void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
void CMtxF__ClampEntries(void *);
void CMtxF__GetTranslation(void *, void *);
void CMtxF__GetScale(void *, void *);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
f32 func_00298470(f32);                 /* sqrtf */

/* Rage Wars' func_802A6174: places one node on its owner, or freezes it when the owner went away (-1). */
static __inline__ void placeNode(Emitter *e, EmitNode *n, CParticle *o) {
    u8 m[0x40];
    f32 k;

    if (o == (CParticle *)-1) {
        n->mtx[gEvenOdd] = n->mtx[gEvenOdd ^ 1];
        n->owner = NULL;
    } else {
        CMtxF__FromMtxAffine(m, o->m_mtxDraw[gEvenOdd]);
        k = 0.01f;
        if (o->m_pEffect->m_pModel != NULL) {
            k = 0.29257143f;
        }
        CMtxF__ScaleAxes(m, k, k, k);
        CMtxF__ClampEntries(m);
        CMtxF__GetTranslation(m, &n->pos);
        if (e->flags & 4) {
            CMtxF__GetScale(m, n->rot);
        } else {
            CMtxF__ToMtxAffine(m, &n->mtx[gEvenOdd]);
        }
    }
}

/* Places every node of the emitter on its owner and sums the distances between consecutive nodes. */
void Trail_PlaceNodes(Emitter *e) {
    CVector3 d;
    EmitNode *n;
    CParticle *o;
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
                CVector3__Subtract(&d, &n->pos, &prev->pos);
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
