#include "common.h"

extern u8 D_8011AB09;
extern u8 D_8011AB0A;
extern u8 D_8011AB0B;
extern u8 D_8011AB0C;
extern u8 D_8011AB0D;
extern u8 D_8011AB0E;
extern u8 D_8011AB0F;
extern u8 D_8011AB11;

/* Whether the unlock flag behind an id is set; ids without a flag count as set. */
s32 func_00262C20(s32 id) {
    switch (id) {
        default:
            return 1;
        case 0x5EB:
        case 0x842:
            return D_8011AB09 == 1;
        case 0x5EC:
        case 0x843:
            return D_8011AB0A == 1;
        case 0x5ED:
        case 0x844:
            return D_8011AB0B == 1;
        case 0x5EE:
        case 0x845:
            return D_8011AB0C == 1;
        case 0x5EF:
        case 0x846:
            return D_8011AB0D == 1;
        case 0x5F0:
        case 0x847:
            return D_8011AB0E == 1;
        case 0x5F1:
        case 0x848:
            return D_8011AB0F == 1;
        case 0x5F3:
        case 0x84A:
            break;
    }
    return D_8011AB11 == 1;
}
