#include "common.h"

/* Pak slot scan step: a pak that already holds one of the game's two files
 * (names D_800B45C0, D_800B45CC) wins at once (*best = 123); otherwise the
 * slot is a candidate when the pak is usable and has room for a save, and
 * the slot with the most free pages is kept. Returns 1 to keep scanning. */

extern s32 D_800C2028;
extern u8 *D_800F70A8;
extern char D_800B45C0[];
extern char D_800B45CC[];

extern void func_00426978(s32 slot, s32 index, char **out);
extern void func_00426C88(char *src, char *dst, s32 n);
extern s32 func_00426D54(char *a, char *b);
extern s32 func_004267A0(s32 slot, s32 *score);
extern s32 func_0042670C(s32 slot, s32 *count);
extern s32 func_00426E6C(u8 *size);

/* func_00421A54 without the index output */
static __inline__ s32 hasFile(char *file, s32 slot) {
    char name[16];
    char *entry;
    s32 i;

    for (i = 0; i < 16; i++) {
        func_00426978(slot, i, &entry);
        func_00426C88(entry, name, 16);
        if (func_00426D54(name, file) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_0041E374(void *unused, s32 slot, s32 *bestSlot, s32 *best) {
    s32 free;
    s32 files;
    s32 found;
    s32 found2;
    s32 err;
    s32 need;

    found = hasFile(D_800B45C0, slot);
    found2 = hasFile(D_800B45CC, slot);
    if (found || found2) {
        *bestSlot = slot;
        *best = 123;
        return 1;
    }
    err = func_004267A0(D_800C2028, &free);
    if (err == 0) {
        err = func_0042670C(D_800C2028, &files);
        if (err == 0) {
            need = func_00426E6C(D_800F70A8 + 0x688);
            if (files == 0 || free < need) {
                if (*best < need) {
                    *best = need;
                    *bestSlot = slot;
                }
                return 0;
            }
        }
    }
    return err == 0;
}
