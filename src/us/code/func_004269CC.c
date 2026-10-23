#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xA];
    /* 0x0A */ u8 name[0x16];
} Slot; /* size 0x20 */

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ Slot slots[16];
} SlotBank; /* size 0x204 */

extern s32 D_80130C60[];
extern SlotBank *D_800C2224;

s32 func_004269CC(s32 bank, s32 slot, u8 **out) {
    if (D_80130C60[bank] != 3) {
        return -2;
    }
    *out = D_800C2224[bank].slots[slot].name;
    return 0;
}
