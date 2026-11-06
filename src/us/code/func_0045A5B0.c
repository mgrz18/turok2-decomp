#include "globals/pak.h"

extern s32 gPakMenuAnyPak;

s32 PakMenu_GetPort_2(char *arg0) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return *(s8 *) (*(char **) (arg0 + 0x20) + 4);
}
