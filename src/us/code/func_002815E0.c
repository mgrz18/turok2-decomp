#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 active;
} Unk0027FC2C;

void *CCameraPool__AddText();                              /* extern */

void *CCameraPool__AddSmallText(void) {
    void *temp_v0;

    temp_v0 = CCameraPool__AddText();
    if (temp_v0 != NULL) {
        ((Unk0027FC2C *)temp_v0)->active = 1;
    }
    return temp_v0;
}
