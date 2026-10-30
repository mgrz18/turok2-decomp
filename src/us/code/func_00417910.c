#include "common.h"

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ char *text;
} MenuItem;

extern u8 D_8011AB08[10]; /* D_8011AAD8 + 0x30: weapons enabled */
extern char D_800C0EB0[]; /* "unknown weapon" */
extern char D_800B6288[];
extern char D_800B62A0[];
extern char D_800B62B8[];
extern char D_800B62D0[];
extern char D_800B62E8[];
extern char D_800B6300[];
extern char D_800B6318[];
extern char D_800B6330[];
extern char D_800B6348[];
extern char D_800B6360[];
extern char D_800B6378[];
extern char D_800B6390[];
extern char D_800B63A8[];
extern char D_800B63C0[];
extern char D_800B63D8[];
extern char D_800B63F0[];

/* Label of a weapon toggle in the weapons menu: "<weapon> on/off". */
s32 func_00417910(MenuItem *a) {
    switch (a->index - 2) {
    default:
        a->text = D_800C0EB0;
        break;
    case 0:
        if (D_8011AB08[1]) {
            a->text = D_800B6288;
        } else {
            a->text = D_800B62A0;
        }
        break;
    case 1:
        if (D_8011AB08[2]) {
            a->text = D_800B62B8;
        } else {
            a->text = D_800B62D0;
        }
        break;
    case 2:
        if (D_8011AB08[3]) {
            a->text = D_800B62E8;
        } else {
            a->text = D_800B6300;
        }
        break;
    case 3:
        if (D_8011AB08[4]) {
            a->text = D_800B6318;
        } else {
            a->text = D_800B6330;
        }
        break;
    case 4:
        if (D_8011AB08[5]) {
            a->text = D_800B6348;
        } else {
            a->text = D_800B6360;
        }
        break;
    case 5:
        if (D_8011AB08[6]) {
            a->text = D_800B6378;
        } else {
            a->text = D_800B6390;
        }
        break;
    case 6:
        if (D_8011AB08[7]) {
            a->text = D_800B63A8;
        } else {
            a->text = D_800B63C0;
        }
        break;
    case 7:
        if (D_8011AB08[9]) {
            a->text = D_800B63D8;
        } else {
            a->text = D_800B63F0;
        }
        break;
    }
    return 0;
}
