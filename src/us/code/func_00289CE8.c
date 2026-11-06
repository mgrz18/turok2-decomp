#include "common.h"
#include "m2c_macros.h"

s32 CEngineApp__UpdateCinemaMode(s32, s32, s32);

extern f32 D_800A9E20;
extern s32 D_800B6D70;
extern s32 D_800F5D2C;
extern s32 D_8011AD00;
extern f32 D_8011B11C;

void EngineMode16_Ending_Retrace(s32 arg0) {
    if (D_800B6D70 != 0) {
        CEngineApp__UpdateCinemaMode(arg0, 1, 1);
        return;
    }
    if ((D_800F5D2C & 0x1000) && (D_8011AD00 == 0)) {
        D_8011AD00 = 1;
        D_800F5D2C = 0;
        D_8011B11C = D_800A9E20;
    }
}
