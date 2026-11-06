/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 gPakMessageKind;
extern char D_800B4BD0[];
extern char D_800B4C10[];
extern char D_800B4C60[];
extern char D_800B4D34[];
extern char D_800B4CCC[];
extern char D_800B4D9C[];
extern char D_800B4E00[];
extern char D_800B4E48[];
extern char D_800B4E74[];
extern char D_800B4EA8[];
extern char D_800B4F04[];
extern char D_800B4F34[];
extern char D_800B4F74[];
extern char D_800B4FAC[];
extern char D_800B4FE8[];
extern char D_800B5024[];
s32 PakMessage_UpdateLine2_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = D_800B4BD0;
        break;
    case 1:
        item->text = D_800B4C10;
        break;
    case 2:
        item->text = D_800B4C60;
        break;
    case 3:
        item->text = D_800B4D34;
        break;
    case 4:
        item->text = D_800B4CCC;
        break;
    case 5:
        item->text = D_800B4D9C;
        break;
    case 6:
        item->text = D_800B4E00;
        break;
    case 7:
        item->text = D_800B4E48;
        break;
    case 8:
        item->text = D_800B4E74;
        break;
    case 9:
        item->text = D_800B4EA8;
        break;
    case 10:
        item->text = D_800B4F04;
        break;
    case 11:
        item->text = D_800B4F34;
        break;
    case 12:
        item->text = D_800B4F74;
        break;
    case 13:
        item->text = D_800B4FAC;
        break;
    case 14:
        item->text = D_800B4FE8;
        break;
    case 15:
        item->text = D_800B5024;
        break;
    }
    return 0;
}
