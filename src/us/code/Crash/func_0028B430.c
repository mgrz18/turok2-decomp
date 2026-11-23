#include "globals/render.h"
#include "common.h"
#include "globals/crash.h"

extern u16 sCrashTextColor; /* local type: globals/frame.h declares it; this view (its type here) is what keeps the ROM's code */          /* foreground colour */

static __inline__ void newline(void) {
    sCrashCursorX = 2;
    sCrashCursorY++;
    if (sCrashCursorY >= gScreenHeight / 16 - 1) {
        sCrashCursorY = 0;
    }
}

/* Debug console: draws one character at the cursor and advances it. */
void Crash_PutChar(s32 c) {
    s32 row;
    s32 col;
    s32 width;
    u16 *fb;
    s32 y;
    s32 x;
    u16 *pixel;

    switch (c) {
    case '\r':
        sCrashCursorX = 2;
        break;
    case '\t':
        sCrashCursorX += 8;
        if (sCrashCursorX >= gScreenWidth / 8 - 1) {
            newline();
        }
        break;
    case '\f':
        sCrashCursorY = 0;
        sCrashCursorX = 2;
        break;
    case '\n':
        newline();
        break;
    default:
        c -= 0x20;
        if (c < 0) {
            c = 0x1F;
        } else if (c >= 0x60) {
            c = 0x1F;
        }
        width = gScreenWidth;
        fb = sCrashFrameBuffer;
        y = sCrashCursorY * 16;
        x = sCrashCursorX * 8;
        /* The row loop is a goto loop: as a for loop GCC hoists the glyph
         * offset out of it, which the ROM does not. */
        row = 0;
    nextRow:
        for (col = 0; col < 8; col++) {
            pixel = (u16 *)(((y + row) * width + (x + col)) * 2 + (uptr)fb);
            if (!((gCrashFont[c][row] >> (7 - col)) & 1)) {
                *pixel = sCrashBgColor;
            } else {
                *pixel = sCrashTextColor;
            }
        }
        if (++row < 16) {
            goto nextRow;
        }
        if (++sCrashCursorX >= gScreenWidth / 8 - 1) {
            newline();
        }
        break;
    }
}
