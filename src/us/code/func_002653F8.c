/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204A68.c, func_00204A68.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

/* Turok 2 layout (Rage Wars offsets in parentheses). */
typedef struct B01Actor {
    char pad0[0x4];
    Vec3f pos;                 /* 0x04 (0x08) */
    char pad10[0x4];
    u8 *def;                   /* 0x14 (0x18) */
    char pad18[0xBA - 0x18];
    s16 unkBA;                 /* 0xBA (s8 0xE6) */
    char padBC[0xD4 - 0xBC];
    u32 flags;                 /* 0xD4 (0x100) */
    f32 unkD8;                 /* 0xD8 (0x104) */
    s16 unkDC;                 /* 0xDC (0x108) */
    s16 unkDE;                 /* 0xDE (0x10A) */
    s16 unkE0;                 /* 0xE0 (0x10C) */
    char padE2[0x100 - 0xE2];
    s16 unk100;                /* 0x100 (0x12C) */
    char pad102[0x108 - 0x102];
    f32 unk108;                /* 0x108 (0x134) */
} B01Actor;

typedef struct B01State {
    char pad0[0xC6];
    u8 unkC6;                  /* 0xC6 (0xCA) */
    s8 unkC7;                  /* 0xC7 (0xCB) */
} B01State;

typedef struct B01Pair {
    s32 a;
    s32 b;
} B01Pair;

extern u8 D_800F7078[];
void func_00237EE4(B01Actor *a, B01Actor *b, s32 kind, Vec3f pos, B01Pair extra);
void func_00220260(void *mgr, B01Actor *actor, s32 flag);

static __inline__ s32 r01_check(B01Actor *actor, B01State *state, f32 scale) {
    if (actor->unkBA == 1) return 1;
    if (actor->unkDC != (s8)state->unkC6 || actor->unkDE != (s8)state->unkC6) return 0;
    if (state->unkC7 != 0 && !(actor->flags & 0x400)) return 1;
    if (actor->unkE0 < 3 && (actor->flags & 0x400)) {
        return (f32)actor->unk100 * scale <= actor->unk108;
    } else {
        return (f32)actor->unkE0 * scale <= actor->unkD8;
    }
}

void func_002653F8(B01Actor *actor, B01State *state) {
    s32 idx = *(s8 *)(actor->def + 0x1D);
    B01Pair extra;
    if (idx != -1) {
        if (r01_check(actor, state, (f32)idx * 0.007874016f)) {
            extra.a = 0;
            func_00237EE4(actor, actor, 7, actor->pos, extra);
            func_00220260(D_800F7078, actor, 0);
        }
    }
}
