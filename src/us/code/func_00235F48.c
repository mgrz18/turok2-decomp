#include "common.h"

s32 SwooshSystem_UpdateSwooshes();                            /* extern */
s32 SwooshSystem_UpdateTimers(s32);                         /* extern */
extern s32 D_800ACFC0;
extern s32 D_8011ACA4;

void SwooshSystem_Update(s32 arg0) {
    D_800ACFC0 = 0;
    if (D_8011ACA4 == 0) {
        SwooshSystem_UpdateSwooshes();
        SwooshSystem_UpdateTimers(arg0);
    }
}
