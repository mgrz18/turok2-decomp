#include "common.h"

/* Walks a chain of offsets relative to the base at 0xC until a zero link. */
void func_00200A74(s32 *arg0) {
    s32 next;
    s32 base;

    next = arg0[0];
    if (next != 0) {
        base = arg0[3];
        do {
            next = *(s32 *)(next + base);
        } while (next != 0);
    }
}
