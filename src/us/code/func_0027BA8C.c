#include "common.h"

/* Builds a 128-entry lookup table: every entry 0xFF (none), then each
 * (index, value) pair of the list, which ends at a negative index. The
 * counter is the first argument, which callers pass for nothing. */
void func_0027BA8C(u8 i, s8 *pairs, u8 *table) {
    s8 *p;
    u8 slot;
    s8 index;

    i = 0;
    do {
        table[i] = 0xFF;
        i++;
    } while ((s8)i >= 0);
    i = 0;
    do {
        p = pairs + i * 2;
        slot = *p;
        index = *p;
        if (index >= 0) {
            table[slot] = p[1];
        }
        i++;
    } while (index >= 0);
}
