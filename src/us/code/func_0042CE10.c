/* WRONG CUT: REAL func_0042CE10 size 0x174 (splat cut it into func_0042CE10 + func_0042CEB0; the word after it, a nop, is padding). */
#include "common.h"

typedef struct Record {
    /* 0x00 */ s32 data[2];
    /* 0x08 */ s32 valid;
    /* 0x0C */ s32 rest[4];
} Record; /* 0x1C */

extern char D_800A5D70[];
extern char D_800A5D80[];
extern void func_0020B490(void);

extern s32 func_00224180(void *level, s32 key);
extern s32 func_00224F84(void *level, s32 id);
extern s32 **func_0020367C(s32 unused, s32 key, s32 size, s32 mode, s32 owner, void *tag, char *name, s32 direct);
extern s32 func_0020185C(s32 *block, s32 key, s32 index, s32 *size);
extern void *func_002017D4(void *table, s32 index);
extern void func_002052D8(s32, void *);

/* Looks key up in the level and loads its record into out (out->valid = 0 when there is none). */
void func_0042CE10(void *level, Record *out, s32 key) {
    s32 id;
    s32 **h1;
    s32 **h2;
    s32 size;
    s32 res;

    id = func_00224180(level, key);
    if (id == -1) {
        out->valid = 0;
        return;
    }
    res = func_00224F84(level, id);
    h1 = func_0020367C(0, res, 0x18, 0, 0, 0, D_800A5D70, 1);
    if (h1 != NULL) {
        h2 = func_0020367C(0, func_0020185C(*h1, res, 1, &size), size, 0, 0, func_0020B490, D_800A5D80, 1);
        if (h2 != NULL) {
            *out = *(Record *)func_002017D4(func_002017D4(func_002017D4(func_002017D4(*h2, 2), 0), 0), 0);
            func_002052D8(0, h2);
        }
        func_002052D8(0, h1);
    }
}
