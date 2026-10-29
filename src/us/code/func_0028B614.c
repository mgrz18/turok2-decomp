#include "common.h"

typedef struct {
    /* 0x0 */ u32 mask;
    /* 0x4 */ u32 value;
    /* 0x8 */ u8 *name;
} FlagDesc;

extern u8 D_800AA0DC[];   /* ": " */
extern u8 D_800AA0E0[];   /* "<" */
extern u8 D_800AA0E4[];   /* "," */
extern u8 D_800AA0E8[];   /* ">\n" */

extern void func_0028B430(s32 c);       /* print a character */
extern void func_0028CD68(s32 byte);    /* print a byte in hex */

static __inline__ void PrintString(u8 *s) {
    if (*s != 0) {
        do {
            func_0028B430(*s++);
        } while (*s != 0);
    }
}

static __inline__ void PrintHex32(u32 value) {
    s32 i;

    for (i = 0; i < 4; i++) {
        func_0028CD68(value >> ((3 - i) * 8));
    }
}

/* Fault screen: prints "name: XXXXXXXX<flag,flag>" with the names of the flags set in value. */
void func_0028B614(u32 value, u8 *name, FlagDesc *desc) {
    s32 first;

    first = 1;
    PrintString(name);
    PrintString(D_800AA0DC);
    PrintHex32(value);
    PrintString(D_800AA0E0);
    while (desc->mask != 0) {
        if ((value & desc->mask) == desc->value) {
            if (first) {
                first = 0;
            } else {
                PrintString(D_800AA0E4);
            }
            PrintString(desc->name);
        }
        desc++;
    }
    PrintString(D_800AA0E8);
}
