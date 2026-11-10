#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 active;
} Unk0027FC2C;

void *CCameraPool__AddText();                              /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CCameraPool__AddText receives CCameraPool__AddSmallText's own argument
 * registers (pass-through); the host passes them. */
void *CCameraPool__AddSmallText(void * pt0, void * pt1, void * pt2) {
    void *temp_v0;

    temp_v0 = CCameraPool__AddText(pt0, pt1, pt2);
    if (temp_v0 != NULL) {
        ((Unk0027FC2C *)temp_v0)->active = 1;
    }
    return temp_v0;
}
#else
void *CCameraPool__AddSmallText(void) {
    void *temp_v0;

    temp_v0 = CCameraPool__AddText();
    if (temp_v0 != NULL) {
        ((Unk0027FC2C *)temp_v0)->active = 1;
    }
    return temp_v0;
}
#endif
