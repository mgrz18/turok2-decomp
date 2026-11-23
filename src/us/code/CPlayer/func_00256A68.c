typedef signed int s32;

s32 CPlayer__TopUpHealth(char *obj) {
    if (*(s32 *) (obj + 0x524) < 0x6400) {
        *(s32 *) (obj + 0x524) = 0x6400;
        return 1;
    }
    return 0;
}
