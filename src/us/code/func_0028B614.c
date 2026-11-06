#include "common.h"
#include "globals/crash.h"

extern u8 D_800AA0DC[];   /* ": " */

extern void Crash_PutChar(s32 c);       /* print a character */
extern void Crash_PrintHex8(s32 byte);    /* print a byte in hex */

static __inline__ void PrintString(u8 *s) {
    if (*s != 0) {
        do {
            Crash_PutChar(*s++);
        } while (*s != 0);
    }
}

static __inline__ void PrintHex32(u32 value) {
    s32 i;

    for (i = 0; i < 4; i++) {
        Crash_PrintHex8(value >> ((3 - i) * 8));
    }
}

/* Fault screen: prints "name: XXXXXXXX<flag,flag>" with the names of the flags set in value. */
void Crash_PrintFlags(u32 value, u8 *name, FlagDesc *desc) {
    s32 first;

    first = 1;
    PrintString(name);
    PrintString(D_800AA0DC);
    PrintHex32(value);
    PrintString("<");
    while (desc->mask != 0) {
        if ((value & desc->mask) == desc->value) {
            if (first) {
                first = 0;
            } else {
                PrintString(",");
            }
            PrintString(desc->name);
        }
        desc++;
    }
    PrintString(">\n");
}
