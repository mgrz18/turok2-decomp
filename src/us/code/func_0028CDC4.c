#include "common.h"

void func_0028B430(s32 c);      /* print a character */
void func_0028CF38(char *str);  /* print a string */

/* Print a signed decimal integer. */
void func_0028CDC4(s32 value) {
    char buf[16];
    char *p;
    s32 n;

    if (value < 0) {
        func_0028B430('-');
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
    func_0028CF38(p);
}
