typedef signed int s32;

typedef struct {
    /* 0x00 */ char pad0[0xE];
    /* 0x0E */ char data[0x12];
} Slot426978;

typedef struct {
    /* 0x000 */ char pad0[0x4];
    /* 0x004 */ Slot426978 slots[16];
} Bank426978; /* size 0x204 */

extern s32 D_80130C60[];
extern Bank426978 *D_800C2224;

s32 func_00426978(s32 bank, s32 slot, char **out) {
    if (D_80130C60[bank] != 3) {
        return -2;
    }
    *out = D_800C2224[bank].slots[slot].data;
    return 0;
}
