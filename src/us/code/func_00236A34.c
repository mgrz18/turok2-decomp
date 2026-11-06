/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7164.c, func_002A7164.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"

typedef struct Emit Emit;
typedef struct EmitDef {
               u8 pad0[2];
               u8 unk2;
               u8 unk3;
               u8 pad4[0x19];
               u8 unk1D;
               u8 pad1E[3];
               u8 unk21;
               u8 unk22;
               u8 unk23;
               s32 unk24;
} EmitDef;
typedef struct EmitOwner {
                u8 pad0[0xF8];
                struct EmitOwnerInfo *info; /* 0xF8 in Turok 2 (0x118 in Rage Wars) */
} EmitOwner;
typedef struct EmitOwnerInfo {
               u8 pad0[0x14];
               s32 unk14;
} EmitOwnerInfo;
typedef struct Mtx64 {
    f32 m[16];
} Mtx64;
typedef struct EmitNode EmitNode;
struct EmitNode {
               u8 pad0[4];
               EmitNode *next;
               f32 unk8;
               u8 padC[4];
               f32 v10[3];
               f32 v1C[3];
               Mtx64 mtx[2];
               f32 unkA8;
               f32 unkAC;
               EmitOwner *unkB0;
};
struct Emit {
               u8 pad0[4];
               Emit *next;
               EmitDef *def;
               s32 unkC;
               s32 unk10;
               u8 pad14[8];
               EmitOwner *owner;
               u8 pad20[4];
               f32 unk24;
               u8 pad28[0xC];
               s32 unk34;
               u8 pad38[4];
               u32 flags;
               EmitNode *list;
               u8 pad44[4];
               s32 unk48;
               f32 unk4C;
};
void CMtxF__ToMtxAffine(void *, void *);
void CMtxF__FromMtxAffine(void *, void *);
void CMtxF__ClampEntries(void *);
void CMtxF__GetTranslation(void *, void *);
void CMtxF__GetScale(void *, void *);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
void func_00236A34(Emit *e, EmitNode *n) {
    u8 m[0x40];
    EmitOwner *o;
    f32 s;
    o = n->unkB0;
    if (o != ((void *)0)) {
        if (o == (EmitOwner *)-1) {
            n->mtx[gEvenOdd] = n->mtx[gEvenOdd ^ 1];
            n->unkB0 = ((void *)0);
        } else {
            CMtxF__FromMtxAffine(m, (u8 *)o + ((gEvenOdd << 6) + 0x48));
            s = 0.01f;
            if (o->info->unk14 != 0) {
                s = 0.29257143f;
            }
            CMtxF__ScaleAxes(m, s, s, s);
            CMtxF__ClampEntries(m);
            CMtxF__GetTranslation(m, n->v10);
            if (e->flags & 4) {
                CMtxF__GetScale(m, n->v1C);
            } else {
                CMtxF__ToMtxAffine(m, &n->mtx[gEvenOdd]);
            }
        }
        n->unk8 = *(f32 *)((u8 *)e->def + 8);
    }
}
