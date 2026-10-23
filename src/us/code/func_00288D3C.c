/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80291054_10.CPP, func_80293904_de, with Turok 2's layouts. */

typedef int s32;
typedef signed char s8;
typedef float f32;

typedef struct {
    char pad0[0x23FD0];
    f32 threshold;              /* 0x23FD0 (0x26DB0 in Rage Wars) */
    char pad23FD4[0x23FDC - 0x23FD4];
    s32 next;                   /* 0x23FDC (0x26DBC) */
    char pad23FE0[1];
    s8 state;                   /* 0x23FE1 (0x26DC1) */
} Context;

extern s32 D_800F5D2C;
extern f32 D_800A9DE4;

s32 func_00288D3C(Context *ctx, f32 value, s32 normalNext, s32 forcedNext) {
    s32 *flag = &D_800F5D2C;

    if (*flag != 0 && forcedNext != -1) {
        ctx->state = 2;
        ctx->next = forcedNext;
        *flag = 0;
        return 1;
    }
    if (value * D_800A9DE4 < ctx->threshold) {
        ctx->state = 2;
        ctx->next = normalNext;
        return 1;
    }
    return 0;
}
