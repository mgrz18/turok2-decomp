#include "common.h"

s32 Music_FadeOut();                            /* extern */
extern s32 D_800F478C[];
extern s32 D_8011ACA0[];

void Music_SetTrack(s32 arg0) {
    if ((D_8011ACA0[0] == 0) && (D_800F478C[0] != arg0)) {
        D_800F478C[0] = arg0;
        if (arg0 == 0) {
            Music_FadeOut();
        }
    }
}
