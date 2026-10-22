/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7480.c, func_002A7480.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
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
                u8 pad0[0x118];
                struct EmitOwnerInfo *info;
                u8 pad11C[0x1F];
                u8 unk13B;
                u8 pad13C[0x9D];
                u8 unk1D9;
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
typedef struct EmitMgr {
                 u8 pad0[0x7520];
                 Emit *head;     /* 0x7520 in Turok 2 (0x7528 in Rage Wars) */
} EmitMgr;
void func_0023417C(Emit *);
void func_00233FCC(Emit *, s32);
void func_00234348(EmitMgr *, Emit *, s32);
void func_00234928(EmitMgr *, Emit *, s32);
/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline void upd(EmitMgr *m, Emit *e, s32 c) {
    if (e->flags & 8) {
        func_0023417C(e);
    }
    if (e->flags & 4) {
        func_00233FCC(e, c);
    }
    if (e->unk48 >= 2) {
        if (e->unk34 >= 0) {
            func_00234348(m, e, c);
        } else {
            func_00234928(m, e, c);
        }
    }
}
void func_00236D50(EmitMgr *m, s32 c) {
    Emit *e;
    for (e = m->head; e != ((void *)0); e = e->next) {
        if (e->unk34 < 0) {
            upd(m, e, c);
        }
    }
    for (e = m->head; e != ((void *)0); e = e->next) {
        if (e->unk34 > 0) {
            upd(m, e, c);
        }
    }
}
