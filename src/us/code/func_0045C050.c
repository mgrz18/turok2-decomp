#include "common.h"

/* The second argument: only its word at 0x10 is read (0 or 1 passes). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 kind;
} Arg1;

extern s32 D_80130940;
extern s32 D_80130998;
extern s32 D_801309AC;

void func_0045C050(s32 arg0, Arg1 *arg1) {
    if ((D_801309AC == 0) && (D_80130998 != 0) && (arg1->kind < 2U)) {
        D_801309AC = 1;
        D_80130940 = 1;
    }
}
