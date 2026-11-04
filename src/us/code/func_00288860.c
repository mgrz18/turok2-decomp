#include "common.h"

void func_00286674(void *);                   /* extern */
s32 __osGetFpcCsr();                                /* extern */
void __osSetFpcCsr(s32);                         /* extern */
void func_0029E3E0(void *, void *, void *, void *); /* extern */
extern u8 D_800A9D3C[];
extern u8 D_800A9D44[];
extern u8 D_800A9D50[];
extern u8 D_800F6CB0[];
extern u8 D_8011F188[];

void mainproc(void) {
    func_0029E3E0(D_8011F188, D_800A9D3C, D_800A9D44, D_800A9D50);
    __osSetFpcCsr(__osGetFpcCsr() | 0x400);
    func_00286674(D_800F6CB0);
}
