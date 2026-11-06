/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 gPakMessageKind;
extern char D_800B4BB8[];
extern char D_800B4BFC[];
extern char D_800B4C48[];
extern char D_800B4D1C[];
extern char D_800B4CB4[];
extern char D_800B4D84[];
extern char D_800B4DEC[];
extern char D_800B4E38[];
extern char D_800B4E70[];
extern char D_800B4E90[];
extern char D_800B4EF8[];
extern char D_800B4F1C[];
extern char D_800B4F68[];
extern char D_800B4F98[];
extern char D_800B4FD4[];
extern char D_800B5010[];
s32 PakMessage_UpdateLine1_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = D_800B4BB8;
        break;
    case 1:
        item->text = D_800B4BFC;
        break;
    case 2:
        item->text = D_800B4C48;
        break;
    case 3:
        item->text = D_800B4D1C;
        break;
    case 4:
        item->text = D_800B4CB4;
        break;
    case 5:
        item->text = D_800B4D84;
        break;
    case 6:
        item->text = D_800B4DEC;
        break;
    case 7:
        item->text = D_800B4E38;
        break;
    case 8:
        item->text = D_800B4E70;
        break;
    case 9:
        item->text = D_800B4E90;
        break;
    case 10:
        item->text = D_800B4EF8;
        break;
    case 11:
        item->text = D_800B4F1C;
        break;
    case 12:
        item->text = D_800B4F68;
        break;
    case 13:
        item->text = D_800B4F98;
        break;
    case 14:
        item->text = D_800B4FD4;
        break;
    case 15:
        item->text = D_800B5010;
        break;
    }
    return 0;
}
