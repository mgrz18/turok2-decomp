#include "common.h"

/* Only the field read here: where to clear a word. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 *flag;
} FlagOwner;

void Rumble_ClearLink(FlagOwner *arg0) {
    s32 *temp_v0;

    temp_v0 = arg0->flag;
    if (temp_v0 != NULL) {
        *temp_v0 = 0;
    }
}
