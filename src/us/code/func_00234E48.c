/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A52E4.c, func_002A52E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"

typedef struct R8Mtx { s32 w[16]; } R8Mtx;
typedef struct R8Info {
               u8 pad0[4];
               f32 unk4;
               f32 unk8;
} R8Info;
typedef struct R8Pawn {
                u8 pad0[0x10F];
                u8 unk10F;      /* 0x10F (Rage Wars 0x13B) */
                u8 pad110[0xA5];
                u8 unk1B5;      /* 0x1B5 (Rage Wars 0x1D9) */
} R8Pawn;
typedef struct R8Part R8Part;
struct R8Part {
               u8 pad0[4];
               R8Part *next;
               f32 unk8;
               u8 padC[4];
               u8 unk10[0x18];
               R8Mtx mtx[2];
               u8 padA8[8];
               s32 owner;
};
typedef struct R8Emit R8Emit;
struct R8Emit {
               u8 pad0[4];
               R8Emit *next;
               R8Info *info;
               u8 padC[0x10];
               R8Pawn *owner;
               u8 pad20[4];
               f32 unk24;
               u8 pad28[0x14];
               u32 flags;
               R8Part *parts;
};
typedef struct R8Cfg { u8 pad0[0x14]; s32 unk14; } R8Cfg;
/* Turok 2 actor: matrices at 0x48, cfg at 0xF8 (Rage Wars 0x60 / 0x118). */
typedef struct R8Actor { u8 pad0[0x48]; R8Mtx mtx[2]; u8 padC8[0x30]; R8Cfg *cfg; } R8Actor;
typedef struct R8Mgr { u8 pad0[0x7520]; R8Emit *list; } R8Mgr;
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
    e = mgr->list;
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
                                CMtxF__FromMtxAffine(sp10, &who->mtx[gEvenOdd]);
                                k = 0.01f;
                                if (who->cfg->unk14 != 0) {
                                    k = 0.29257143f;
                                }
                                CMtxF__ScaleAxes(sp10, k, k, k);
                                CMtxF__ClampEntries(sp10);
                                CMtxF__GetTranslation(sp10, p->unk10);
                                if (e->flags & 4) {
                                    CMtxF__GetScale(sp10, (u8 *)p + 0x1C);
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
              register s32 t asm("$2") = gEvenOdd;
              register u8 *wm asm("$5") = (u8 *)((t << 6) + (sptr)who);
              __asm__("" : "+r"(wm));
              if (e->unk24 > 0.0f) {
                wm += 0x48;
                CMtxF__FromMtxAffine(sp10, wm);
                k = 0.01f;
                if (who->cfg->unk14 != 0) {
                    k = 0.29257143f;
                }
                CMtxF__ScaleAxes(sp10, k, k, k);
                CMtxF__ClampEntries(sp10);
                Swoosh_NewNode(gEngineApp_m_SwooshSystem, e, sp10);
              }
            }
            if (e->owner != ((void *)0)) {
                if (e->flags & 1) {
                    if (e->owner->unk10F != 0) {
                        e->owner->unk10F = e->owner->unk10F - 1;
                    }
                }
                if (e->flags & 2) {
                    if (e->owner->unk1B5 != 0) {
                        e->owner->unk1B5 = e->owner->unk1B5 - 1;
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
