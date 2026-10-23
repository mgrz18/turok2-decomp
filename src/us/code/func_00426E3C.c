typedef signed int s32;
typedef unsigned char u8;

s32 func_00426E3C(u8 *str, s32 len) {
    do {
        if (*str++ == '~') {
            return 1;
        }
    } while (--len != -1);
    return 0;
}
