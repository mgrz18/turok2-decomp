/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A7164.c, func_002A7164, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "globals/engine.h"
#include "structs/swoosh.h"

void CMtxF__ToMtxAffine(void *mf, void *mtx);
void CMtxF__FromMtxAffine(void *mf, void *mtx);
void CVector3__Subtract(void *out, void *a, void *b);
void CMtxF__ClampEntries(void *mf);
void CMtxF__GetTranslation(void *mf, void *out);
void CMtxF__GetScale(void *mf, void *out);
void CMtxF__ScaleAxes(void *mf, f32 x, f32 y, f32 z);
f32 func_00298470(f32);                 /* sqrtf */

/* Rage Wars' func_802A6174: places one node on its owner, or freezes it when the owner went away (-1). */
static __inline__ void placeNode(Swoosh *e, SwooshNode *n, CParticle *o) {
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
            CMtxF__GetScale(m, &n->scale);
        } else {
            CMtxF__ToMtxAffine(m, &n->mtx[gEvenOdd]);
        }
    }
}

/* Places every node of the swoosh on its owner and sums the distances between consecutive nodes. */
void Trail_PlaceNodes(Swoosh *e) {
    CVector3 d;
    SwooshNode *n;
    CParticle *o;
    SwooshNode *prev;
    f32 len;
    s32 moved;

    n = e->nodes.head;
    moved = 0;
    e->length = 0.0f;
    if (n != NULL) {
        do {
            o = n->owner;
            if (o != NULL) {
                placeNode(e, n, o);
                moved = 1;
                n->life = e->info->nodeLife;
            }
            prev = n->prev;
            if (prev != NULL) {
                CVector3__Subtract(&d, &n->pos, &prev->pos);
                len = func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
                prev->length = len;
                e->length += len;
            }
            n = n->next;
        } while (n != NULL);
    }
    if (moved) {
        e->time = e->info->unk0;
    }
}
