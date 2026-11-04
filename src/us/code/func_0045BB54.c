/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_801309C0;
extern char D_800B4BF4[];
extern char D_800B4C28[];
extern char D_800B4C90[];
extern char D_800B4D64[];
extern char D_800B4CFC[];
extern char D_800B4DC8[];
extern char D_800B4E2C[];
extern char D_800B4E64[];
extern char D_800B4E88[];
extern char D_800B4ED4[];
extern char D_800B4F14[];
extern char D_800B4F5C[];
extern char D_800B4F90[];
extern char D_800B4FCC[];
extern char D_800B5008[];
extern char D_800B5054[];
s32 func_0045BB54(MenuItem *item) {
    switch (D_801309C0) {
    default:
    case 0:
        item->text = D_800B4BF4;
        break;
    case 1:
        item->text = D_800B4C28;
        break;
    case 2:
        item->text = D_800B4C90;
        break;
    case 3:
        item->text = D_800B4D64;
        break;
    case 4:
        item->text = D_800B4CFC;
        break;
    case 5:
        item->text = D_800B4DC8;
        break;
    case 6:
        item->text = D_800B4E2C;
        break;
    case 7:
        item->text = D_800B4E64;
        break;
    case 8:
        item->text = D_800B4E88;
        break;
    case 9:
        item->text = D_800B4ED4;
        break;
    case 10:
        item->text = D_800B4F14;
        break;
    case 11:
        item->text = D_800B4F5C;
        break;
    case 12:
        item->text = D_800B4F90;
        break;
    case 13:
        item->text = D_800B4FCC;
        break;
    case 14:
        item->text = D_800B5008;
        break;
    case 15:
        item->text = D_800B5054;
        break;
    }
    return 0;
}
