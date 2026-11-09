/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A52E4.c, func_002A52E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

typedef struct R8Mtx { s32 w[16]; } R8Mtx;
typedef struct R8Info {
               u8 pad0[4];
               f32 unk4;
               f32 unk8;
} R8Info;
typedef struct R8Part R8Part;
/* structs/swoosh.h SwooshNode, Swoosh: the same layouts. */
struct R8Part {
               R8Part *prev;
               R8Part *next;
               f32 unk8;
               u8 padC[4];
               u8 unk10[0x18];
               R8Mtx mtx[2];
               u8 padA8[8];
               sptr owner;
};
typedef struct R8Emit R8Emit;
struct R8Emit {
               R8Emit *prev;
               R8Emit *next;
               R8Info *info;
               void *tex0;
               void *tex1;
               s32 texture[2];
               void *owner;     /* flags & 1: a CGameObjectInstance, & 2: a CParticle */
               s32 id;
               f32 unk24;
               f32 unk28;
               f32 unk2C;
               f32 unk30;
               s32 layer;
               s32 unk38;
               u32 flags;
               R8Part *parts;
};
/* The particle a swoosh follows: m_mtxDraw at 0x48, m_pEffect at 0xF8 (Rage Wars 0x60 / 0x118). */
typedef CParticle R8Actor;
typedef CSwooshSystem R8Mgr;
extern u8 gEngineApp_m_SwooshSystem[];
void CMtxF__ToMtxAffine(void *, void *);
void CMtxF__FromMtxAffine(void *, void *);
void CMtxF__ClampEntries(void *);
void CMtxF__GetTranslation(void *, void *);
void CMtxF__GetScale(void *, void *);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
void Swoosh_NewNode(void *, void *, void *);
void Trail_DetachOwner(R8Mgr *mgr, R8Actor *who) {
    u8 sp10[0x40];
    R8Emit *e;
    R8Part *p;
    f32 k;
    s32 i;
    u8 *dst, *src;
    e = mgr->m_SwooshPool.used.head;
    if (e == ((void *)0)) {
        return;
    }
    do {
        if (e->flags & 8) {
            p = e->parts;
            if (p != ((void *)0)) {
                do {
                    if (p->owner == (sptr)who) {
                        if (who != ((void *)0)) {
                            if ((sptr)who == -1) {
                                p->mtx[gEvenOdd] = p->mtx[gEvenOdd ^ 1];
                                p->owner = 0;
                            } else {
                                CMtxF__FromMtxAffine(sp10, who->m_mtxDraw[gEvenOdd]);
                                k = 0.01f;
                                if (who->m_pEffect->m_pModel != 0) {
                                    k = 0.29257143f;
                                }
                                CMtxF__ScaleAxes(sp10, k, k, k);
                                CMtxF__ClampEntries(sp10);
                                CMtxF__GetTranslation(sp10, p->unk10);
                                if (e->flags & 4) {
                                    CMtxF__GetScale(sp10, p->unk10 + 0xC);
                                } else {
                                    CMtxF__ToMtxAffine(sp10, &p->mtx[gEvenOdd]);
                                }
                            }
                            p->unk8 = e->info->unk8;
                        }
                        p->owner = -1;
                    }
                    p = p->next;
                } while (p != ((void *)0));
            }
        } else if ((R8Actor *)e->owner == who) {
            if (e->flags & 2) {
#ifdef TARGET_PORT
              u8 *wm = who->m_mtxDraw[gEvenOdd];
              if (e->unk24 > 0.0f) {
#else
              register s32 t asm("$2") = gEvenOdd;
              register u8 *wm asm("$5") = (u8 *)((t << 6) + (sptr)who);
              __asm__("" : "+r"(wm));
              if (e->unk24 > 0.0f) {
                wm += 0x48;
#endif
                CMtxF__FromMtxAffine(sp10, wm);
                k = 0.01f;
                if (who->m_pEffect->m_pModel != 0) {
                    k = 0.29257143f;
                }
                CMtxF__ScaleAxes(sp10, k, k, k);
                CMtxF__ClampEntries(sp10);
                Swoosh_NewNode(gEngineApp_m_SwooshSystem, e, sp10);
              }
            }
            if (e->owner != ((void *)0)) {
                if (e->flags & 1) {
                    if (((CGameObjectInstance *)e->owner)->m_ActiveSwooshes != 0) {
                        ((CGameObjectInstance *)e->owner)->m_ActiveSwooshes = ((CGameObjectInstance *)e->owner)->m_ActiveSwooshes - 1;
                    }
                }
                if (e->flags & 2) {
                    if (((CParticle *)e->owner)->m_ActiveSwooshes != 0) {
                        ((CParticle *)e->owner)->m_ActiveSwooshes = ((CParticle *)e->owner)->m_ActiveSwooshes - 1;
                    }
                }
            }
            e->owner = ((void *)0);
            if (e->unk24 < e->info->unk4) {
                e->unk24 = e->info->unk4;
            }
        }
        e = e->next;
    } while (e != ((void *)0));
}
