typedef int s32;

extern s32 gPakState[4];
extern s32 gPakError[4];

s32 Pak_NeedsRepair_2(s32 index) {
    if (gPakState[index] != 3) {
        return 0;
    }
    return gPakError[index] == -4;
}
