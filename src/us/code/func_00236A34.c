/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7164.c, func_002A7164.
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
extern f32 D_800A6534;
extern f32 D_800A6538;
extern s32 D_800B6D1C;
void func_0020D274(void *, void *);
void func_0020D908(void *, void *);
void func_0020F7EC(void *);
void func_002101A0(void *, void *);
void func_002101BC(void *, void *);
void func_0021034C(void *, f32, f32, f32);
void func_00236A34(Emit *e, EmitNode *n) {
    u8 m[0x40];
    EmitOwner *o;
    f32 s;
    o = n->unkB0;
    if (o != ((void *)0)) {
        if (o == (EmitOwner *)-1) {
            n->mtx[D_800B6D1C] = n->mtx[D_800B6D1C ^ 1];
            n->unkB0 = ((void *)0);
        } else {
            func_0020D908(m, (u8 *)o + ((D_800B6D1C << 6) + 0x48));
            s = D_800A6534;
            if (o->info->unk14 != 0) {
                s = D_800A6538;
            }
            func_0021034C(m, s, s, s);
            func_0020F7EC(m);
            func_002101A0(m, n->v10);
            if (e->flags & 4) {
                func_002101BC(m, n->v1C);
            } else {
                func_0020D274(m, &n->mtx[D_800B6D1C]);
            }
        }
        n->unk8 = *(f32 *)((u8 *)e->def + 8);
    }
}
