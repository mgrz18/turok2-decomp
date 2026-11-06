/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char D_800B4BF8[];
extern char D_800B4C3C[];
extern char D_800B4CA8[];
extern char D_800B4D7C[];
extern char D_800B4D14[];
extern char D_800B4DE0[];
extern char D_800B4E34[];
extern char D_800B4E6C[];
extern char D_800B4E8C[];
extern char D_800B4EEC[];
extern char D_800B4F18[];
extern char D_800B4F64[];
extern char D_800B4F94[];
extern char D_800B4FD0[];
extern char D_800B500C[];
extern char D_800B506C[];
s32 PakMessage_UpdateLine5_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = D_800B4BF8;
        break;
    case 1:
        item->text = D_800B4C3C;
        break;
    case 2:
        item->text = D_800B4CA8;
        break;
    case 3:
        item->text = D_800B4D7C;
        break;
    case 4:
        item->text = D_800B4D14;
        break;
    case 5:
        item->text = D_800B4DE0;
        break;
    case 6:
        item->text = D_800B4E34;
        break;
    case 7:
        item->text = D_800B4E6C;
        break;
    case 8:
        item->text = D_800B4E8C;
        break;
    case 9:
        item->text = D_800B4EEC;
        break;
    case 10:
        item->text = D_800B4F18;
        break;
    case 11:
        item->text = D_800B4F64;
        break;
    case 12:
        item->text = D_800B4F94;
        break;
    case 13:
        item->text = D_800B4FD0;
        break;
    case 14:
        item->text = D_800B500C;
        break;
    case 15:
        item->text = D_800B506C;
        break;
    }
    return 0;
}
