#include "common.h"

s32 CAudioWorld__Initialize(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Sound__Initialize(void) {
    CAudioWorld__Initialize(&D_800F2A00);
}
