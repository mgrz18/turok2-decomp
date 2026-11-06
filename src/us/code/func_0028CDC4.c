#include "common.h"

void Crash_PutChar(s32 c);      /* print a character */
void Crash_PrintString(char *str);  /* print a string */

/* Print a signed decimal integer. */
void Crash_PrintInt(s32 value) {
    char buf[16];
    char *p;
    s32 n;

    if (value < 0) {
        Crash_PutChar('-');
        n = -value;
    } else {
        n = value;
    }
    p = &buf[15];
    *p = 0;
    do {
        *--p = n % 10 + '0';
        n /= 10;
    } while (n != 0);
    Crash_PrintString(p);
}
