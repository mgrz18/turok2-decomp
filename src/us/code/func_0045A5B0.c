typedef signed int s32;
typedef signed char s8;

extern s32 D_80130990;
extern s32 D_800C2028;

s32 PakMenu_GetPort_2(char *arg0) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return *(s8 *) (*(char **) (arg0 + 0x20) + 4);
}
