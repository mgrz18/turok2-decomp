/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00263E1C.c, func_00263E1C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/controller.h"

void func_00284EB8(ControllerProfile *c, s8 x, s8 y, s32 btn) {
    u32 b;
    f32 fx;
    register f32 fy asm("$f1");
    s32 prev;
    s32 pressed;
    s32 m;
    s32 i;
    u8 old;
    ControllerButtonHistory *e;
    if (c->connected != 0) {
        b = btn & 0xFFFF;
        if (x < -30) {
            b |= 0x20000;
        } else if (x >= 31) {
            b |= 0x40000;
        }
        if (y < -30) {
            b |= 0x80000;
        } else if (y >= 31) {
            b |= 0x10000;
        }
        if (x > 0) {
            x = x - 3;
            if (x < 0) {
                x = 0;
            }
        } else {
            x = x + 3;
            if (x > 0) {
                x = 0;
            }
        }
        if (y > 0) {
            y = y - 3;
            if (y < 0) {
                y = 0;
            }
        } else {
            y = y + 3;
            if (y > 0) {
                y = 0;
            }
        }
        fx = x;
        fy = y;
        if (fx == -128.0f) {
            fx = -127.0f;
        }
        if (fy == -128.0f) {
            fy = -127.0f;
        }
        if (fx < -80.0f) {
            fx = -80.0f;
        }
        if (80.0f < fx) {
            fx = 80.0f;
        }
        if (fy < -80.0f) {
            fy = -80.0f;
        }
        if (80.0f < fy) {
            fy = 80.0f;
        }
        prev = c->inputButtons;
        c->inputStickX = (s32)fx;
        c->inputStickY = (s32)fy;
        c->inputButtons = b;
        c->inputPreviousButtons = prev;
        pressed = (prev ^ b) & b;
        c->inputButtonsSingle |= pressed;
        c->inputButtonsRepeat |= pressed;
        m = 1;
        e = c->buttonHistory;
        i = 0x13;
        do {
            if (pressed & m) {
                if (e->age < 0x1E) {
                    e->age = 0;
                    e->taps++;
                    if (e->taps >= 2) {
                        c->inputButtonsDouble |= m;
                        e->taps = 0;
                    }
                } else {
                    e->taps = 1;
                    e->age = 0;
                }
            }
            e->age++;
            if (e->age >= 0x1F) {
                e->age = 0x1E;
            }
            if (b & m) {
                if ((u8)e->held < 0xFF) {
                    e->held += 1;
                }
                old = e->repeat;
                e->repeat = old - 1;
                if (e->repeat < 0) {
                    e->repeat = old + 4;
                    c->inputButtonsRepeat |= m;
                }
            } else {
                if ((u8)e->held < 0x11 && (c->inputPreviousButtons & m)) {
                    c->inputButtonsSingleNoHold |= m;
                }
                e->held = 0;
                c->inputButtonsRepeat &= ~m;
                e->repeat = 0x30;
            }
            e++;
            m <<= 1;
        } while (i-- != 0);
    }
}
