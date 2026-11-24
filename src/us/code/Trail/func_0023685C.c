#include "common.h"
#include "structs/swoosh.h"


void Material_GetTextureSet(void *texture, s32 id);
void CParticle__AddDrawModeBits(void *particle, void *texture);

/* Loads swoosh arg1's texture set (its record's id, -1: untextured) and adds its draw mode to particle arg2. */
void Trail_LoadTexture(s32 arg0, Swoosh *arg1, s32 arg2) {
    s32 temp_a1;
    void *temp_s0;

    temp_a1 = arg1->layer;
    if (temp_a1 != -1) {
        temp_s0 = arg1->texture;
        Material_GetTextureSet(temp_s0, temp_a1);
        if (arg2 != 0) {
            CParticle__AddDrawModeBits((void *) (sptr) arg2, temp_s0);
        }
    }
}
