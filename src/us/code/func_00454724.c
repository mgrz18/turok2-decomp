/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DA30.c, func_0043DA30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u8 pad[8]; u32 unk8; u8 pad2[8]; void *unk14; } O06A;
extern s32 D_8011AADC;
extern s32 D_8011AAD8;
extern u8 D_800B5A14[];
extern u8 D_800B5A30[];
s32 func_00454724(O06A *a) {
    if (D_8011AADC & 0x800) a->unk8 |= 0x1000000; else a->unk8 &= ~0x1000000;
    if (D_8011AAD8 & 0x800) a->unk14 = D_800B5A14; else a->unk14 = D_800B5A30;
    return 0;
}
