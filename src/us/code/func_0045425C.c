/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D568.c, func_0043D568.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u8 pad[8]; u32 unk8; u8 pad2[8]; void *unk14; } O06A;
extern s32 D_8011AADC;
extern s32 D_8011AAD8;
extern u8 D_800B581C[];
extern u8 D_800B5838[];
s32 func_0045425C(O06A *a) {
    if (D_8011AADC & 4) a->unk8 |= 0x1000000; else a->unk8 &= ~0x1000000;
    if (D_8011AAD8 & 4) a->unk14 = D_800B581C; else a->unk14 = D_800B5838;
    return 0;
}
