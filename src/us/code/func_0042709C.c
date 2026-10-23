typedef unsigned char u8;

/* In-place upper-casing of a NUL-terminated string. */
u8 *func_0042709C(u8 *str) {
    u8 *p = str;

    if (str == 0) {
        return 0;
    }
    while (*p != 0) {
        if ((u8) (*p - 'a') < 26) {
            *p -= 0x20;
        }
        p++;
    }
    return str;
}
