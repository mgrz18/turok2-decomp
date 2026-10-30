#include "common.h"

s32 func_00224C08(void *arg0, s32 arg1);
s32 func_004160F0(void *item);

extern u8 D_800F7078[];
extern char D_800B438C[];
extern char D_800B43A8[];
extern char D_800B43FC[];
extern char D_800B4454[];
extern char D_800B44B4[];
extern char D_800B44E8[];
extern char D_800B454C[];
extern s32 D_80130A30;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00424B90(MenuItem *item) {
    s32 rank = -1;

    switch (D_80130A30) {
        case -1:
        case 0:
        default:
            item->text = D_800B438C;
            break;
        case 1:
            item->text = D_800B43A8;
            rank = func_00224C08(D_800F7078, 2);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 2:
            item->text = D_800B43FC;
            rank = func_00224C08(D_800F7078, 4);
            if (rank != -1) {
                rank = 2 - rank;
            }
            break;
        case 3:
            item->text = D_800B4454;
            rank = func_00224C08(D_800F7078, 6);
            if (rank != -1) {
                rank = 5 - rank;
            }
            break;
        case 4:
            item->text = D_800B44B4;
            rank = func_00224C08(D_800F7078, 8);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 5:
            item->text = D_800B44E8;
            rank = func_00224C08(D_800F7078, 9);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 6:
            item->text = D_800B454C;
            rank = func_00224C08(D_800F7078, 11);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
    }
    if (rank != -1) {
        item->text[func_004160F0(item) - 6] = rank + '0';
    }
    return 0;
}
