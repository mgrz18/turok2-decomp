/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DCE4.c, func_0043DCE4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef struct { u8 pad[0x510]; s32 unk510; } O06B; /* Rage Wars: 0x5D0 */
typedef struct { u8 pad[0x1C]; O06B *unk1C; } O06A;
extern s32 D_801198A0;
extern u8 D_800F6CB0[];
extern void func_00430250(void *a);
s32 func_00453334(s32 a, O06A *b) {
    if (D_801198A0 == 1) func_00430250(D_800F6CB0); else b->unk1C->unk510 = 0;
    return 1;
}
