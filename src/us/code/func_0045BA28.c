/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_801309C0;
extern char D_800B4BE8[];
extern char D_800B4C18[];
extern char D_800B4C78[];
extern char D_800B4D4C[];
extern char D_800B4CE4[];
extern char D_800B4DB0[];
extern char D_800B4E14[];
extern char D_800B4E58[];
extern char D_800B4E78[];
extern char D_800B4EBC[];
extern char D_800B4F10[];
extern char D_800B4F48[];
extern char D_800B4F84[];
extern char D_800B4FBC[];
extern char D_800B4FF8[];
extern char D_800B503C[];
s32 func_0045BA28(MenuItem *item) {
    switch (D_801309C0) {
    default:
    case 0:
        item->text = D_800B4BE8;
        break;
    case 1:
        item->text = D_800B4C18;
        break;
    case 2:
        item->text = D_800B4C78;
        break;
    case 3:
        item->text = D_800B4D4C;
        break;
    case 4:
        item->text = D_800B4CE4;
        break;
    case 5:
        item->text = D_800B4DB0;
        break;
    case 6:
        item->text = D_800B4E14;
        break;
    case 7:
        item->text = D_800B4E58;
        break;
    case 8:
        item->text = D_800B4E78;
        break;
    case 9:
        item->text = D_800B4EBC;
        break;
    case 10:
        item->text = D_800B4F10;
        break;
    case 11:
        item->text = D_800B4F48;
        break;
    case 12:
        item->text = D_800B4F84;
        break;
    case 13:
        item->text = D_800B4FBC;
        break;
    case 14:
        item->text = D_800B4FF8;
        break;
    case 15:
        item->text = D_800B503C;
        break;
    }
    return 0;
}
