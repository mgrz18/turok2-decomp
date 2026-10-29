#include "common.h"

extern u8 D_800AA0F0[];           /* "\n" */
extern u8 D_800AA0F4[];           /* "Call stack " */
extern u8 D_800AA100[];           /* "(top:" */
extern u8 D_800AA108[];           /* " bot:" */
extern u8 D_800AA110[];           /* " low:" */
extern u8 D_800AA118[];           /* ")\n" */
extern u16 D_8011F184;            /* debug console foreground colour */

void func_0028B430(s32 c);          /* debug console: draws one character */
void func_0028CD68(s32 byte);       /* debug console: draws a byte in hex */
s32 func_00267024(u32 address);   /* nonzero for a code address */

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

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 *sp;             /* the saved stack pointer (low word) */
} Thread;

/* Debug console: dumps a thread's stack, its unused low part (filled with
 * fill) and the first five words on it that look like return addresses. */
void func_0028BDFC(u8 *stack, s32 size, Thread *thread, u8 fill) {
    u8 *low;
    u32 *top;
    u32 *sp;
    s32 count;

    top = (u32 *)(stack + size);
    low = stack;
    while (*low == fill) {
        low++;
    }
    putString(D_800AA0F0);
    putString(D_800AA0F4);
    putString(D_800AA100);
    putHex32((u32)(stack + size));
    putString(D_800AA108);
    putHex32((u32)stack);
    putString(D_800AA110);
    putHex32((u32)low);
    putString(D_800AA118);
    D_8011F184 = 0xF7A9;
    for (sp = thread->sp, count = 1; sp < top && count < 6; sp++) {
        if (func_00267024(*sp)) {
            putHex32(*sp);
            putString(D_800AA0F0);
            count++;
        }
    }
    D_8011F184 = 0xA529;
}
