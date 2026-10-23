typedef int s32;

typedef struct {
    char pad0[0xE];
    char data[0x12];            /* 0x0E */
} Slot;                         /* size 0x20 */

typedef struct {
    char pad0[0x4];
    Slot slots[16];             /* 0x004 */
} Bank;                         /* size 0x204 */

extern s32 D_80130C60[4];
extern Bank *D_800C2224;

s32 func_0045F898(s32 bank, s32 slot, char **out) {
    if (D_80130C60[bank] != 3) {
        return -2;
    }
    *out = D_800C2224[bank].slots[slot].data;
    return 0;
}
