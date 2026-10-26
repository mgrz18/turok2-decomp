#include "common.h"

/* D_8011AAD8: game flags block (D_80119870 + 0x1268). */
typedef struct {
    u8 pad0[0x19];
    u8 f19;          /* 0x19 */
    u8 f1A;          /* 0x1A */
    u8 pad1B[0x4D];
    u8 f68;          /* 0x68 */
    u8 pad69[0x1B3];
    s32 f21C;        /* 0x21C (D_8011ACB0 + 0x44) */
} GameFlags;

extern u8 D_8011ACB0[];
extern u8 D_800F7078[];

void func_002888BC(s32, s32, s32);
s32 func_0042AC0C(void *, s32, s32);

void func_002892D8(s32 arg0) {
    u8 *base = D_8011ACB0;
    GameFlags *flags;

    *(s32 *) (base + 0x44) = 0;
    flags = (GameFlags *) (base - 0x1D8); /* D_8011AAD8 */
    flags->f68 = 1;
    flags->f19 = 0;
    flags->f1A = 1;
    func_002888BC(arg0, 0, 0);
    func_0042AC0C(D_800F7078, 0, 0);
}
