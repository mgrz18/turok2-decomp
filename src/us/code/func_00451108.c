/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E520.c, func_0043E520.
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
               u8 pad0[0x2B];
               s8 b2B;
} S10Sub;
typedef struct S10Ctx {
                u8 pad0[0x518];
                S10Sub *sub;    /* player options, 0x518 in Turok 2 */
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
typedef struct S10Item {
               u8 pad0[0x14];
               void *f14;
} S10Item;
extern u8 D_800B4210[];
extern u8 D_800B4220[];
s32 func_00451108(S10Item *a, S10Menu *m) {
    switch (m->ctx->sub->b2B) {
    case 0:
        a->f14 = D_800B4210;
        break;
    case 1:
        a->f14 = D_800B4220;
        break;
    }
    return 0;
}
