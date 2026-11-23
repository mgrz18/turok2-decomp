#include "common.h"

extern u8 sCrashText_ColonSpace[];           /* ": " */
extern u8 sCrashText_Spaces3[];           /* "   " */
extern u8 sCrashText_Newline[];           /* "\n" */

void Crash_PutChar(s32 c);          /* debug console: draws one character */
void Crash_PrintHex8(s32 byte);       /* debug console: draws a byte in hex */

static __inline__ void putString(u8 *s) {
    while (*s) {
        Crash_PutChar(*s++);
    }
}

static __inline__ void putHex32(u32 value) {
    s32 i;

    for (i = 0; i < 4; i++) {
        Crash_PrintHex8(value >> ((3 - i) * 8));
    }
}

/* Debug console: prints four labelled 32-bit values in hex on one line. */
void Crash_PrintRegs4(u8 *name0, u32 value0, u8 *name1, u32 value1, u8 *name2, u32 value2,
                   u8 *name3, u32 value3) {
    putString(name0);
    putString(sCrashText_ColonSpace);
    putHex32(value0);
    putString(sCrashText_Spaces3);
    putString(name1);
    putString(sCrashText_ColonSpace);
    putHex32(value1);
    putString(sCrashText_Spaces3);
    putString(name2);
    putString(sCrashText_ColonSpace);
    putHex32(value2);
    putString(sCrashText_Spaces3);
    putString(name3);
    putString(sCrashText_ColonSpace);
    putHex32(value3);
    putString(sCrashText_Newline);
}
