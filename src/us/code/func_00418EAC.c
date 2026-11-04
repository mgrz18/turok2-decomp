#include "common.h"
#include "structs/menu.h"

extern u8 D_8011AAFC; /* D_8011AAD8 + 0x24 */
extern char D_800B6164[];
extern char D_800B617C[];

s32 func_00418EAC(MenuItem *a) {
    switch (D_8011AAFC) {
    case 0:
        a->text = D_800B617C;
        break;
    case 1:
        a->text = D_800B6164;
        break;
    }
    return 0;
}
