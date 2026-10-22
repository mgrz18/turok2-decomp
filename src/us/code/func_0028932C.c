/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293F38.c, func_00293F38.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

/* Turok 2 layout: the timer sits at 0x23FD0 (0x26DB0 in Rage Wars). */
typedef struct B21Game {
    u8 pad0[0x23FD0];
    f32 timer23FD0;
} B21Game;
extern s32 D_800B6D68;
extern s32 D_800B6D6C;
extern f32 D_800A9DFC;
u32 func_00266B80();
s32 func_00288D3C(B21Game *g, f32 x, s32 a, s32 b);
void func_0028932C(B21Game *g) {
    s32 mode;
    u32 r = func_00266B80();
    mode = 4;
    if (r > 0x400000) {
        if (D_800B6D68 != 0) {
            mode = 3;
        }
    }
    if (g->timer23FD0 > D_800A9DFC) {
        D_800B6D6C = 0;
    }
    if (D_800B6D6C != 0) {
        if (func_00288D3C(g, 12.0f, mode, -1)) {
            D_800B6D6C = 0;
        }
    } else {
        if (func_00288D3C(g, 12.0f, mode, mode)) {
            D_800B6D6C = 0;
        }
    }
}
