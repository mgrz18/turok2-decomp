typedef int s32;

extern s32 gPakState[4]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 gPakError[4]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */

s32 Pak_NeedsRepair_2(s32 index) {
    if (gPakState[index] != 3) {
        return 0;
    }
    return gPakError[index] == -4;
}
