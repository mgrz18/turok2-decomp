/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7F58.c, func_802A7554_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern char D_800B75F0;
extern s32 D_80120690;
extern char D_8011F290;
extern char D_8011AAD8;
extern char D_800EBAC0;
extern char D_800E6AC0;
extern char D_8011ACB0;
/* Turok 2 region sizes differ from Rage Wars (0x430, 0x584, 0xB0). */
s32 func_0028D714(u32 address) {
    address |= 0x80000000;
    if (((address) >= (u32)&(D_800B75F0) && (address) < (u32)&(D_800B75F0) + (0x10)) || address == (u32)&D_80120690) {
        return 0;
    }
    if (D_80120690 != 0) {
        return 1;
    }
    if (((address) >= (u32)&(D_8011F290) && (address) < (u32)&(D_8011F290) + (0x1400))) {
        return 0;
    }
    if (((address) >= (u32)&(D_8011AAD8) && (address) < (u32)&(D_8011AAD8) + (0x184))) {
        return 0;
    }
    if (((address) >= (u32)&(D_800EBAC0) && (address) < (u32)&(D_800EBAC0) + (0x5800))) {
        return 0;
    }
    if (((address) >= (u32)&(D_800E6AC0) && (address) < (u32)&(D_800E6AC0) + (0x5000))) {
        return 0;
    }
    if (address >= (u32)&D_8011ACB0 && address <= (u32)&D_8011ACB0 + 0x4C) {
        return 0;
    }
    return 1;
}
