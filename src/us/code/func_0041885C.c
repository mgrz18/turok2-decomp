#include "common.h"
#include "structs/menu.h"

extern u8 D_8011AB01; /* D_8011AAD8 + 0x29 */
extern char D_800B6164[];
extern char D_800B617C[];

s32 FragTagMenu_CrosshairsLabel(MenuItem *a) {
    switch (D_8011AB01) {
    case 0:
        a->text = D_800B617C;
        break;
    case 1:
        a->text = D_800B6164;
        break;
    }
    return 0;
}
