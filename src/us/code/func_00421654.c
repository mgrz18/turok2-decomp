typedef signed int s32;

extern char D_0043BEF4[];
extern char D_0043BED0[];
extern s32 D_80130940;

s32 func_00421654(char *arg0) {
    char *p = *(char **) (arg0 + 0x24);

    if (p == D_0043BEF4 || p == D_0043BED0) {
        D_80130940 = 0;
        return 1;
    }
    return 0;
}
