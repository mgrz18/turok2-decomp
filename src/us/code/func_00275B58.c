#include "common.h"

s32 CAudioWorld__DetachAllVoices(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Sound__DetachAll(void) {
    CAudioWorld__DetachAllVoices(&D_800F2A00);
}
