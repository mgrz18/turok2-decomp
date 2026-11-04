#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 active;
} Unk0027FC2C;

void *func_0027FC2C();                              /* extern */

void *func_002815E0(void) {
    void *temp_v0;

    temp_v0 = func_0027FC2C();
    if (temp_v0 != NULL) {
        ((Unk0027FC2C *)temp_v0)->active = 1;
    }
    return temp_v0;
}
