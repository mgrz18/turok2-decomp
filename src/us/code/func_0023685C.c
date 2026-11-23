#include "common.h"

/* A swoosh trail (see Trail_Draw). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 model[0x34 - 0x14];
    /* 0x34 */ s32 texture;
} Trail;

s32 Material_GetTextureSet(void *, s32);                 /* extern */
s32 CParticle__AddDrawModeBits(sptr, void *);                 /* extern */

void Trail_LoadTexture(s32 arg0, Trail *arg1, s32 arg2) {
    s32 temp_a1;
    void *temp_s0;

    temp_a1 = arg1->texture;
    if (temp_a1 != -1) {
        temp_s0 = arg1->model;
        Material_GetTextureSet(temp_s0, temp_a1);
        if (arg2 != 0) {
            CParticle__AddDrawModeBits(arg2, temp_s0);
        }
    }
}
