#include "common.h"

/* Menu item: append the rank title earned for the number of kills (11 chars). */

typedef struct {
    u8 pad0[0x14];
    char *text; /* 0x14 */
} MenuItem;

extern u16 D_8013094C; /* kills */
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

s32 func_0045B3A4(MenuItem *item) {
    s32 kills = D_8013094C;
    char *rank = NULL;
    char *dst;
    s32 i;

    if (kills >= 0 && kills < 50) {
        rank = D_800B4934;
    } else if (kills >= 50 && kills < 150) {
        rank = D_800B4940;
    } else if (kills >= 150 && kills < 300) {
        rank = D_800B494C;
    } else if (kills >= 300 && kills < 500) {
        rank = D_800B4958;
    } else if (kills >= 500 && kills < 800) {
        rank = D_800B4964;
    } else if (kills >= 800 && kills < 1300) {
        rank = D_800B4970;
    } else if (kills >= 1300 && kills < 2000) {
        rank = D_800B497C;
    } else if (kills >= 2000 && kills < 3000) {
        rank = D_800B4988;
    } else if (kills >= 3000 && kills < 4500) {
        rank = D_800B4994;
    } else if (kills >= 4500 && kills < 6500) {
        rank = D_800B49A0;
    } else if (kills >= 6500 && kills < 9999) {
        rank = D_800B49AC;
    } else if (kills >= 9999) {
        rank = D_800B49B8;
    }
    dst = item->text;
    dst += 8;
    if (rank != NULL) {
        for (i = 0; i < 11; i++) {
            *dst++ = *rank++;
        }
    }
    return 0;
}
