typedef signed int s32;
typedef signed char s8;
typedef unsigned short u16;

s32 func_00260360(char *arg0, char *arg1) {
    char *p = *(char **) (arg0 + 0x14) + 0x14;
    s8 c = *(s8 *) (arg1 + 0x34);

    if (c == 0x20 || c == 0x3D) {
        return 0;
    }
    switch (*(s8 *) (arg1 + 0x133)) {
        case 0:
            return (*(u16 *) (p + 0x7E) & 0x1F) != 0;
        case 1:
            return (*(u16 *) (p + 0x7E) & 0x3E0) != 0;
        case 2:
            return (*(u16 *) (p + 0x7E) & 0x7C00) != 0;
    }
    return 0;
}
