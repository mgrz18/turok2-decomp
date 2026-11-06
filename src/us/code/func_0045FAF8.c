typedef int s32;

extern s32 D_80130C60[4];
extern s32 D_80130C70[4];

s32 Pak_NeedsRepair_2(s32 index) {
    if (D_80130C60[index] != 3) {
        return 0;
    }
    return D_80130C70[index] == -4;
}
