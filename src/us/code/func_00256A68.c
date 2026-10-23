typedef signed int s32;

s32 func_00256A68(char *obj) {
    if (*(s32 *) (obj + 0x524) < 0x6400) {
        *(s32 *) (obj + 0x524) = 0x6400;
        return 1;
    }
    return 0;
}
