#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s16 id;
} Entry;

/* Up to three entries at 0x20. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Entry *entries[3];
} EntrySet;

extern EntrySet *D_800AF618[];

/* Whether set arg1 lists id arg2 (the list stops at the first NULL). */
s32 func_00256B34(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    EntrySet *p = D_800AF618[arg1];
    for (i = 0; i < 3; i++) {
        Entry *e = p->entries[i];
        if (e != NULL) {
            if (e->id == arg2) {
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}
