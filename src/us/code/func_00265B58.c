/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205324.c, func_00205324.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef union M1IF { s32 i; f32 f; } M1IF;
typedef union M1W4 { s32 i; f32 f; s16 h[2]; u16 hu[2]; u8 b[4]; u32 u; } M1W4;
typedef union M1B1 { s8 s; u8 u; } M1B1;
typedef struct M1Info M1Info;
typedef struct M1Def M1Def;
typedef struct M1Actor M1Actor;
typedef struct M1State M1State;
typedef struct M1Ref M1Ref;
typedef struct M1Stats M1Stats;
typedef struct M1Pad M1Pad;
struct M1Info {
                u32 flags;
                M1W4 unk4;
                s8 b8;
                u8 pad9[0x3];
                s32 unkC;
                u8 pad10[0x8];
                s32 unk18;
                s32 unk1C;
                M1W4 unk20;
                M1W4 unk24;
                M1W4 unk28;
                f32 f2C;
                f32 f30;
                f32 f34;
                u8 pad38[0x4];
                f32 f3C;
                M1W4 unk40;
                f32 f44;
                M1W4 unk48;
                M1W4 unk4C;
                u8 b50[4];
                f32 f54;
                f32 f58;
                f32 f5C;
                f32 f60;
                f32 f64;
                f32 f68;
                u8 pad6C[0x4];
                u8 b70;
                u8 b71;
};
struct M1Def {
                s32 kind;
                u32 unk4;
                u8 pad8[0xC];
                M1Info info;
};
struct M1Actor { /* Turok 2 layout */
                u8 pad0[0x4];
                Vec3f pos;          /* 0x04 */
                u8 pad10[0x4];
                M1Def *def;         /* 0x14 */
                u8 pad18[0xBC];
                u32 flags;          /* 0xD4 (Rage Wars: 0x100) */
};
struct M1State {
                u8 pad0[0x4];
                s32 unk4;
                s32 unk8;
                u8 padC[0x28];
                s8 unk34;
                s8 unk35;
                s8 unk36;
                s8 unk37;
                u8 pad38[0x4];
                u32 flags3C;
                f32 f40;
                f32 f44;
                Vec3f v48;
                Vec3f v54;
                f32 f60;
                f32 f64;
                u8 pad68[0x18];
                M1Actor *unk80;
                u8 pad84[0x18];
                f32 f9C;
                Vec3f vA0;
                u8 padAC[0x1E];
                u8 unkCA;
                s8 unkCB;
                u8 padCC[0x1];
                u8 unkCD;
                u8 padCE[0x2E];
                M1Ref *unkFC;
                u8 pad100[0x8];
                void (*fn108)();
                u8 pad10C[0x18];
                M1IF unk124;
                M1IF unk128;
                M1IF unk12C;
                Vec3f vec130;
                f32 f13C;
};
struct M1Ref {
                u8 pad0[0xC];
                s32 unkC;
};
struct M1Stats {
                u8 pad0[0x8];
                u16 h8;
                u8 padA[0x85];
                u8 b8F;
                u8 b90;
                u8 pad91[0x1];
                u8 b92;
};
struct M1Pad {
                u8 pad0[0xC6];
                M1B1 stickX;
                M1B1 stickY;
};
extern s32 D_800F7078[];
extern u8 D_801102B8[];
void func_00220260(s32 *p, M1Actor *a, s32 n);
void func_0026E110(M1Actor *a, s32 b, M1Actor *c);
void func_00275544(s16 id, Vec3f pos, s32 a, s32 b);
void func_00246690(M1Actor *a, s32 id, Vec3f pos, s32 n);
void func_0024646C(M1Actor *a, M1State *s, s32 n, s32 x, s32 y);
void func_002365F8(u8 *p, M1Actor *a);
void func_00265B58(M1Actor *actor, M1State *state) {
    M1Info *info = &actor->def->info;
    s32 mode;
    func_00220260(D_800F7078, actor, 1);
    func_0026E110(actor, 0x40000, actor);
    if (state->unk4 == 0) {
        actor->flags &= ~0x10000;
    }
    if (info->unk18 == 0) {
        actor->flags &= ~0x2000;
        actor->flags &= ~0x100;
    }
    mode = D_800F7078[0];
    if (mode == 4) {
        if (info->unk20.i != -1) {
            func_00275544(info->unk20.h[1], actor->pos, 0, -1);
        }
        if (info->unk1C != -1) {
            func_00246690(actor, info->unk1C, actor->pos, 0);
        }
        func_0024646C(actor, state, 4, info->unk24.i, info->unk28.i);
        func_002365F8(D_801102B8, actor);
        if (D_800F7078[0] != mode) goto L;
    } else {
L:
        actor->flags &= ~0x100;
        actor->flags |= 0x8000000;
    }
}
