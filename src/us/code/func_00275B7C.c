#include "common.h"

s32 CAudioWorld__Lock(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Sound__Lock(void) {
    CAudioWorld__Lock(&D_800F2A00);
}
