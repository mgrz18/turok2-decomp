#include "common.h"

s32 CAudioWorld__DuckMusic(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Music_DuckVolume(void) {
    CAudioWorld__DuckMusic(&D_800F2A00);
}
