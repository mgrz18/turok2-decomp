typedef signed int s32;

typedef struct {
    /* 0x00 */ char pad0[0xA];
    /* 0x0A */ char data[0x16];
} Entry; /* size 0x20 */

typedef struct {
    /* 0x000 */ char pad0[0x4];
    /* 0x004 */ Entry entries[16];
} Block; /* size 0x204 */

extern s32 D_80130C60[];
extern Block *D_800C2224;

s32 func_0045F8EC(s32 i, s32 j, char **out) {
    if (D_80130C60[i] != 3) {
        return -2;
    }
    *out = D_800C2224[i].entries[j].data;
    return 0;
}
