#include "common.h"

void CEngineApp__Main(void *);                   /* extern */
s32 __osGetFpcCsr();                                /* extern */
void __osSetFpcCsr(s32);                         /* extern */
void sprintf(void *, void *, void *, void *); /* extern */
extern u8 D_800A9D3C[];
extern u8 D_800A9D44[];
extern u8 D_800A9D50[];
extern u8 D_800F6CB0[];
extern u8 D_8011F188[];

void mainproc(void) {
    sprintf(D_8011F188, D_800A9D3C, D_800A9D44, D_800A9D50);
    __osSetFpcCsr(__osGetFpcCsr() | 0x400);
    CEngineApp__Main(D_800F6CB0);
}
