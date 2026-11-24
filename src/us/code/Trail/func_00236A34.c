/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7164.c, func_002A7164.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * On structs/swoosh.h's Swoosh and SwooshNode; the owner is a CParticle. */
#include "common.h"
#include "globals/engine.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

void CMtxF__ToMtxAffine(void *mf, void *mtx);
void CMtxF__FromMtxAffine(void *mf, void *mtx);
void CMtxF__ClampEntries(void *mf);
void CMtxF__GetTranslation(void *mf, void *out);
void CMtxF__GetScale(void *mf, void *out);
void CMtxF__ScaleAxes(void *mf, f32 x, f32 y, f32 z);

/* Moves point n to its owner particle's draw matrix (-1: the owner is gone, the point keeps the
 * other frame buffer's matrix), scaled to world units (0.2926 for a model particle). */
void Swoosh_UpdateNodeFromOwner(Swoosh *e, SwooshNode *n) {
    u8 m[0x40];
    CParticle *o;
    f32 s;
    o = n->owner;
    if (o != ((void *)0)) {
        if (o == (CParticle *)-1) {
            n->mtx[gEvenOdd] = n->mtx[gEvenOdd ^ 1];
            n->owner = ((void *)0);
        } else {
            CMtxF__FromMtxAffine(m, o->m_mtxDraw[gEvenOdd]);
            s = 0.01f;
            if (o->m_pEffect->m_pModel != 0) {
                s = 0.29257143f;
            }
            CMtxF__ScaleAxes(m, s, s, s);
            CMtxF__ClampEntries(m);
            CMtxF__GetTranslation(m, &n->pos);
            if (e->flags & 4) {
                CMtxF__GetScale(m, &n->scale);
            } else {
                CMtxF__ToMtxAffine(m, &n->mtx[gEvenOdd]);
            }
        }
        n->life = e->info->nodeLife;
    }
}
