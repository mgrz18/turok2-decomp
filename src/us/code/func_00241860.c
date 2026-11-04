#include "common.h"

s32 func_002418C0(s32 *);                   /* extern */
extern s32 D_0042D510;
extern s32 *D_800F1ADC;
extern s32 D_800F1CE0;

void CCollisionInfo__Reset(void) {
    D_800F1ADC = &D_800F1CE0;
    func_002418C0(&D_0042D510);
}
