#include "common.h"
#include "globals/strings.h"


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

/* Fault screen: prints two registers on one line, "name1: XXXXXXXX   name2: XXXXXXXX". */
void Crash_PrintRegPair(u8 *name1, u32 value1, u8 *name2, u32 value2) {
    PrintString(name1);
    PrintString(sCrashText_ColonSpace);
    PrintHex32(value1);
    PrintString(sCrashText_Spaces3);
    PrintString(name2);
    PrintString(sCrashText_ColonSpace);
    PrintHex32(value2);
    PrintString(sCrashText_Newline);
}
