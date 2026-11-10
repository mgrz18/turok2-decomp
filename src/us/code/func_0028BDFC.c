#include "common.h"
#include "globals/frame.h"

extern u8 D_800AA0F0[];           /* "\n" */

void Crash_PutChar(s32 c);          /* debug console: draws one character */
void Crash_PrintHex8(s32 byte);       /* debug console: draws a byte in hex */
s32 IsCodePointer(uptr address);   /* nonzero for a code address */

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

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 *sp;             /* the saved stack pointer (low word) */
} Thread;

/* Debug console: dumps a thread's stack, its unused low part (filled with
 * fill) and the first five words on it that look like return addresses. */
void Crash_PrintStack(u8 *stack, s32 size, Thread *thread, u8 fill) {
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
    putString("Call stack ");
    putString("(top:");
    putHex32((uptr)(stack + size));
    putString(" bot:");
    putHex32((uptr)stack);
    putString(" low:");
    putHex32((uptr)low);
    putString(")\n");
    sCrashTextColor = 0xF7A9;
    for (sp = thread->sp, count = 1; sp < top && count < 6; sp++) {
        if (IsCodePointer(*sp)) {
            putHex32(*sp);
            putString(D_800AA0F0);
            count++;
        }
    }
    sCrashTextColor = 0xA529;
}
