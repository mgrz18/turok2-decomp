#include "common.h"

s32 CAudioWorld__FadeOutMusic(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Music_FadeOut(void) {
    CAudioWorld__FadeOutMusic(&D_800F2A00);
}
