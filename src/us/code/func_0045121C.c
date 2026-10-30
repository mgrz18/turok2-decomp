/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E65C.c, func_0043E65C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct S10Ctrl {
               s32 active;
               s8 port;
               u8 pad5[0x224 - 5];
} S10Ctrl;
typedef struct S10Sub {
               u8 pad0[0x30];
               s8 b30;        /* 0x30 (player options) */
} S10Sub;
typedef struct S10Ctx {
                u8 pad0[0x518];
                S10Sub *sub;    /* 0x518: player options */
                u8 *f51C;
                s32 f520;
} S10Ctx;
typedef struct S10Flags {
               u8 pad0[0x58];
               u32 flags;
} S10Flags;
typedef struct S10Menu {
               s16 state;
               u8 pad2[0xA];
               S10Flags *fC;
               u8 pad10[0xC];
               S10Ctx *ctx;
               S10Ctrl *ctrl;
} S10Menu;
typedef struct S10Pair {
               s16 v;
               s16 w;
} S10Pair;
extern s32 func_002855E8(void *p);
extern s32 func_00416558(S10Menu *a, s32 cur, s32 step, s32 lo, s32 hi, s32 wrap);
extern void func_0042E350(S10Ctx *c, s32 v);
extern void func_00416214(void *a, s32 b, s32 c);
extern void func_00275624(s32 a);
extern void func_00416894(s32 a, s32 b, void *c);
extern u8 D_00438668[];
extern S10Pair D_800C1E7A[];
s32 func_0045121C(void *x, S10Menu *m, void *y) {
    S10Ctx *ctx = m->ctx;
    S10Sub *sub = ctx->sub;
    if (func_002855E8(m->ctrl)) {
        func_0042E350(ctx, sub->b30);
        func_00275624(D_800C1E7A[ctx->f520].v);
        func_00416894((s32)y, (s32)m, D_00438668);
        return 1;
    } else {
        s8 v = func_00416558(m, sub->b30, 1, 0, 0xA, 1);
        sub->b30 = v;
        func_0042E350(ctx, v);
        func_00416214(x, 0xB, sub->b30);
        return 0;
    }
}
