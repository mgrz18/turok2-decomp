#include "common.h"

s32 CAudioWorld__StopMusic(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Music_Stop(void) {
    CAudioWorld__StopMusic(&D_800F2A00);
}
