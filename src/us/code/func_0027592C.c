#include "common.h"

s32 CAudioWorld__Update(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Sound__Update(void) {
    CAudioWorld__Update(&D_800F2A00);
}
