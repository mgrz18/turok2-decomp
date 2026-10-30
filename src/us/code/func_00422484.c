/* WRONG CUT: one function 0x00422484-0x00422608 (size 0x184); splat cut its tail (the last range
 * tests and the copy loop) into func_00422574. */
#include "common.h"

extern char D_800B4934[];
extern char D_800B4940[];
extern char D_800B494C[];
extern char D_800B4958[];
extern char D_800B4964[];
extern char D_800B4970[];
extern char D_800B497C[];
extern char D_800B4988[];
extern char D_800B4994[];
extern char D_800B49A0[];
extern char D_800B49AC[];
extern char D_800B49B8[];
extern u16 D_8013094C; /* score */

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00422484(MenuItem *item) {
    s32 score = D_8013094C;
    char *src = NULL;
    char *dst;
    s32 i;

    if (score >= 0 && score < 50) {
        src = D_800B4934;
    } else if (score >= 50 && score < 150) {
        src = D_800B4940;
    } else if (score >= 150 && score < 300) {
        src = D_800B494C;
    } else if (score >= 300 && score < 500) {
        src = D_800B4958;
    } else if (score >= 500 && score < 800) {
        src = D_800B4964;
    } else if (score >= 800 && score < 1300) {
        src = D_800B4970;
    } else if (score >= 1300 && score < 2000) {
        src = D_800B497C;
    } else if (score >= 2000 && score < 3000) {
        src = D_800B4988;
    } else if (score >= 3000 && score < 4500) {
        src = D_800B4994;
    } else if (score >= 4500 && score < 6500) {
        src = D_800B49A0;
    } else if (score >= 6500 && score < 9999) {
        src = D_800B49AC;
    } else if (score >= 9999) {
        src = D_800B49B8;
    }
    dst = item->text;
    dst += 8;
    if (src != NULL) {
        for (i = 0; i < 11; i++) {
            *dst++ = *src++;
        }
    }
    return 0;
}
