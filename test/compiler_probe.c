/*
 * Compiler probe: eight engine functions written back to C that compile
 * byte-exact through tools/sn64/cc1 -O2 (SN64 GCC 2.8.1), relocations aside.
 *
 * This is the evidence that settles which compiler built the engine, and it
 * doubles as a regression test for the toolchain:
 *
 *     python3 tools/match_func.py test/compiler_probe.c
 *
 * must report MATCH for every function. The structs are the minimum each
 * function touches, named by offset; they are not the engine's real types.
 * The functions were picked at random from .code, sized 24-120 bytes.
 */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

extern char D_800D8450[];
extern void func_0029B820(void *, s32, s32);
void func_0020578C(s32 unused, s32 x) {
    func_0029B820(D_800D8450, x, 1);
}

extern void func_00255FF4(void);
void func_00256148(void) {
    func_00255FF4();
}

extern char D_800F2A00[];
extern void func_00271514(void *);
void func_00275BA0(void) {
    func_00271514(D_800F2A00);
}

typedef struct { char pad[0x210]; f32 key; } Sortable;
s32 func_002837B0(Sortable **a, Sortable **b) {
    s32 r = 1;
    if ((*a)->key < (*b)->key) {
        r = -1;
    }
    return r;
}

typedef struct { s32 a; s32 b; s32 c; u16 d; } Src29;
typedef struct { char pad0[8]; s32 a; s32 b; char pad10[0xA]; u16 d; } Dst29;
void func_0029321C(Dst29 *dst, Src29 *src) {
    dst->a = src->a;
    dst->d = src->d;
    dst->b = src->b;
}

typedef struct { char pad[8]; u8 *p; } Reader;
u32 func_002932BC(Reader *r) {
    u32 v;
    u32 c;
    c = *r->p++;
    v = c;
    if (v & 0x80) {
        v &= 0x7F;
        do {
            v <<= 7;
            c = *r->p++;
            v += c & 0x7F;
        } while (c & 0x80);
    }
    return v;
}

typedef struct { char pad[0x1C]; s32 x1C; char pad2[8]; s32 x28; char pad3[4]; } Slot;
typedef struct { char pad[0x3C]; s32 cur; Slot *slots; } Pool;
void func_00293530(Pool *p, s16 i) {
    if (p->slots[i].x28 == 0) {
        p->slots[i].x1C = 0;
        if (p->cur == i) {
            p->cur = -1;
        }
    }
}

extern f32 D_800A7948;
extern f32 D_800B6D28;
extern void func_00243414(void *, void *, s32);
typedef struct { char pad[0x64]; f32 v; } Obj64;
void func_0025FA54(void *self, Obj64 *o) {
    if (o->v > D_800A7948) {
        o->v = D_800A7948;
    }
    o->v -= D_800B6D28;
    if (o->v <= 0.0f) {
        func_00243414(self, o, 10);
    }
}
