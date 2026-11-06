#include "common.h"
#include "globals/frame.h"

extern u8 D_800AA0DC[];   /* ": " */
extern u8 D_800AA0EC[];   /* "   " */
extern u8 D_800AA0F0[];   /* "\n" */


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

/* Fault screen: like Crash_PrintRegPair, but prints the second register in another colour. */
void Crash_PrintRegPairHighlight(u8 *name1, u32 value1, u8 *name2, u32 value2) {
    PrintString(name1);
    PrintString(D_800AA0DC);
    PrintHex32(value1);
    PrintString(D_800AA0EC);
    sCrashTextColor = 0xF7A9;
    PrintString(name2);
    PrintString(D_800AA0DC);
    PrintHex32(value2);
    sCrashTextColor = 0xA529;
    PrintString(D_800AA0F0);
}
