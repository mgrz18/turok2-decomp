#include "common.h"

extern u8 D_800AA0DC[];   /* ": " */
extern u8 D_800AA0EC[];   /* "   " */
extern u8 D_800AA0F0[];   /* "\n" */

extern u16 D_8011F184;     /* debug console foreground colour */

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

/* Fault screen: like func_0028B7C0, but prints the second register in another colour. */
void func_0028B954(u8 *name1, u32 value1, u8 *name2, u32 value2) {
    PrintString(name1);
    PrintString(D_800AA0DC);
    PrintHex32(value1);
    PrintString(D_800AA0EC);
    D_8011F184 = 0xF7A9;
    PrintString(name2);
    PrintString(D_800AA0DC);
    PrintHex32(value2);
    D_8011F184 = 0xA529;
    PrintString(D_800AA0F0);
}
