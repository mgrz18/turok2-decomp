#include "common.h"

extern u8 D_800AA0DC[];           /* ": " */
extern u8 D_800AA0EC[];           /* "   " */
extern u8 D_800AA0F0[];           /* "\n" */

void func_0028B430(s32 c);          /* debug console: draws one character */
void func_0028CD68(s32 byte);       /* debug console: draws a byte in hex */

static __inline__ void putString(u8 *s) {
    while (*s) {
        func_0028B430(*s++);
    }
}

static __inline__ void putHex32(u32 value) {
    s32 i;

    for (i = 0; i < 4; i++) {
        func_0028CD68(value >> ((3 - i) * 8));
    }
}

/* Debug console: prints four labelled 32-bit values in hex on one line. */
void func_0028BB00(u8 *name0, u32 value0, u8 *name1, u32 value1, u8 *name2, u32 value2,
                   u8 *name3, u32 value3) {
    putString(name0);
    putString(D_800AA0DC);
    putHex32(value0);
    putString(D_800AA0EC);
    putString(name1);
    putString(D_800AA0DC);
    putHex32(value1);
    putString(D_800AA0EC);
    putString(name2);
    putString(D_800AA0DC);
    putHex32(value2);
    putString(D_800AA0EC);
    putString(name3);
    putString(D_800AA0DC);
    putHex32(value3);
    putString(D_800AA0F0);
}
