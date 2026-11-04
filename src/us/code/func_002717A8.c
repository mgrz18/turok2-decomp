#include "common.h"

/* Only the field read here: a table of 12-byte entries. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2B78];
    /* 0x2B78 */ s32 entries;
} EntryOwner;

/* The address of entry arg1. */
s32 func_002717A8(EntryOwner *arg0, s32 arg1) {
    return arg0->entries + (arg1 * 0xC);
}
