typedef signed int s32;

typedef struct {
    /* 0x00 */ s32 used;
    /* 0x04 */ char pad4[0x1C];
} Entry; /* size 0x20 */

typedef struct {
    /* 0x000 */ char pad0[0x4];
    /* 0x004 */ Entry entries[16];
} Block; /* size 0x204 */

extern s32 D_80130C60[];
extern s32 D_80130C70[];
extern Block *D_800C2224;

s32 func_0045F62C(s32 i, s32 *freeCount) {
    s32 j;

    if (D_80130C60[i] != 3) {
        return -2;
    }
    *freeCount = 0;
    if (D_80130C70[i] == 0) {
        for (j = 0; j < 16; j++) {
            if (D_800C2224[i].entries[j].used == 0) {
                (*freeCount)++;
            }
        }
    }
    return D_80130C70[i];
}
