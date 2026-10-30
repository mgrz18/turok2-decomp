/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444314_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
struct MenuItemRecAt14;
struct MenuItemRecAt14 {
    char pad0[8];
    u32 flags;
    char padC[8];
    void *table;
};
struct MenuItemRecAt14;
typedef struct MenuItemRecAt14 MenuItemRecAt14;
extern u8 D_800B3E88[];
extern u8 D_800B3EA0[];
extern s32 D_8011AC88;
extern u8 D_800B3E70[];
extern u32 D_8011AAE0[];
s32 func_00454DBC(MenuItemRecAt14 *item) {
    if (D_8011AC88 == 8) {
        item->flags |= 0x01000000;
    } else {
        item->flags &= ~0x01000000;
    }
    switch (D_8011AAE0[0]) {
    case 8:
        item->table = D_800B3E70;
        break;
    case 0x10:
        item->table = D_800B3E88;
        break;
    case 0x20:
        item->table = D_800B3EA0;
        break;
    }
    return 0;
}
