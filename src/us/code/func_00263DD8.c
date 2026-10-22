/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00206DD4.c, func_00206DD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
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
/* Turok 2 LevelActor / state: only the fields used here. */
struct M1Actor {
    u8 pad0[0x4];
    Vec3f pos;      /* 0x4 */
    s32 unk10;      /* 0x10 */
    M1Def *def;     /* 0x14 */
    u8 pad18[0x38];
    f32 f50;        /* 0x50 (0x6C in Rage Wars) */
};
struct M1State {
    u8 pad0[0x3C];
    u32 flags3C;    /* 0x3C */
    u8 pad40[0x14];
    Vec3f v54;      /* 0x54 */
    f32 f60;
    f32 f64;        /* 0x64 */
    u8 pad68[0x30];
    f32 f98;        /* 0x98 */
    Vec3f v9C;      /* 0x9C */
    u8 padA8[0x68];
    M1IF unk110;    /* 0x110 */
    M1IF unk114;
    Vec3f vec118;
    f32 f124;       /* 0x124 */
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
f32 func_00266F50(f32 x);
void func_0020EF8C(Vec3f *dst, f32 t, Vec3f *src, Vec3f *v);
f32 func_0026BE60(s32 id, f32 x, f32 z);
void func_0026C888(s32 id, f32 y);
void func_00263DD8(M1Actor *actor, M1State *state) {
    M1Info *info;
    f32 t;
    f32 lo;
    f32 x;
    f32 fs0;
    f32 c;
    Vec3f v;
    t = state->f64;
    lo = 0.0f;
    info = &actor->def->info;
    if (t < lo || (lo = 1.0f, lo < t)) {
        t = lo;
    }
    if (info->unk28.b[1] == 0) {
        t = func_00266F50(t);
    }
    v = state->v9C;
    v.y += info->f2C;
    if (info->unk4.hu[0] != 0) {
        v.x += info->f30 * func_002982F0(info->f34);
        v.z += info->f30 * func_002974C0(info->f34);
    }
    func_0020EF8C(&state->v54, t, &state->v9C, &v);
    switch (info->unk28.b[0]) {
    case 0:
        x = func_002982F0(state->f124);
        goto join;
    case 1:
        fs0 = state->f124;
        c = 0.25f;
        state->v54.y += info->f44 * (func_002982F0(fs0) * c);
        state->v54.y += info->f44 * (func_002982F0(fs0 * 1.1f) * c);
        state->v54.y += info->f44 * (func_002982F0(fs0 * 1.2f) * c);
        x = func_002982F0(fs0 * 1.3f) * c;
join:
        state->v54.y += info->f44 * x;
        break;
    }
    state->v54.x += state->unk110.f;
    state->v54.y += state->unk114.f;
    state->v54.z += state->vec118.x;
    switch (info->unk28.b[2]) {
    case 0:
        actor->f50 = state->f98 + t * ((state->f98 + info->f3C) - state->f98);
        break;
    case 1:
        if (!(state->flags3C & 0x400)) {
    case 2:
            if (info || (&actor->pos)) {
                actor->f50 = actor->f50 + info->f3C * 0.06666667f;
            } else {
                actor->f50 = actor->f50 + info->f3C * 0.06666667f;
            }
        }
        break;
    }
    if (info->unk4.hu[0] == 0) {
        func_0026C888(actor->unk10, (func_0026BE60(actor->unk10, actor->pos.x, actor->pos.z) + state->v54.y) - actor->pos.y);
    }
}
