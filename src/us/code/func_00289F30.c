/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00294BB0.c, func_00294BB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
/* Turok 2 layouts (Rage Wars offsets in parentheses). */
typedef struct B21Game {
    u8 pad0[0x23FF4];
    s32 unk23FF4;       /* 0x23FF4 (0x26DD4) */
} B21Game;
typedef struct B21GameBlk {
    u8 pad0[0x44];
    s32 unk44;          /* 0x44 (0x88) */
} B21GameBlk;
typedef struct B21World {
    u8 pad0[0x5DC];
    u8 sub5DC[4];       /* 0x5DC */
    u8 pad5E0[0xE80];
    B21GameBlk blk;     /* 0x1460 (0x1860) */
} B21World;
extern B21World D_80119850;
extern s32 D_8011ACF4;
s32 func_00416AF8(void *a);
s32 Cinema_PathPlaying();
s32 func_00284408();
s32 func_00284240();
void func_00412B14();
s32 func_00289F30(B21Game *g) {
    B21GameBlk *b;
    s32 r;
    if (func_00416AF8(D_80119850.sub5DC) == 0) {
        if (Cinema_PathPlaying()) {
            if (func_00284408() == 0x78) {
                b = &D_80119850.blk;
                if (b->unk44 != 0) {
                    goto check;
                }
                if (g->unk23FF4 == 0) {
                    b->unk44 = 1;
                    func_00412B14();
                }
            }
        }
    }
    if (D_8011ACF4 != 0) {
check:
        r = func_00284240();
        if (r) return 1;
    }
    return 0;
}
