#include "common.h"

/* Only the field read here: the key of the resource this loads. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 key;
} ResourceOwner;

s32 CISet__GetROMAddressAndSize(s32, s32, s32, s32 *);            /* extern */
s32 *CCache__Request(s32, s32, s32, s32, s32, void *, void *, s32); /* extern */
void CCache__Unlock(s32, s32 *);                     /* extern */
void Model_TouchTextures(s32 *);                          /* extern */
extern u8 D_800A5858[];
extern u8 D_800A5870[];
extern void Model_DecompressCallbackTerminated();

void CLevelActor__TouchGeometry(ResourceOwner *arg0) {
    s32 sp20;
    s32 *temp_v0;
    s32 *var_s0;
    s32 temp_s0;

    temp_v0 = CCache__Request(0, arg0->key, 0x18, 4, 0, NULL, D_800A5858 + 4, 0);
    if (temp_v0 != NULL) {
        temp_s0 = CISet__GetROMAddressAndSize(*temp_v0, arg0->key, 1, &sp20);
        CCache__Unlock(0, temp_v0);
        var_s0 = CCache__Request(0, temp_s0, sp20, 4, 0, Model_DecompressCallbackTerminated, D_800A5870, 0);
    } else {
        var_s0 = NULL;
    }
    if (var_s0 != NULL) {
        Model_TouchTextures(var_s0);
        CCache__Unlock(0, var_s0);
    }
}
