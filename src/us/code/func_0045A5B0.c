typedef signed int s32;
typedef signed char s8;

extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;

s32 PakMenu_GetPort_2(char *arg0) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return *(s8 *) (*(char **) (arg0 + 0x20) + 4);
}
