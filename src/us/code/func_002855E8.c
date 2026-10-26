#include "common.h"

/* What this reads of the controller state. */
typedef struct {
    /* 0x00 */ u8 _pad0[0xB4];
    /* 0xB4 */ u32 m_Pressed;   /* N64 button bits: A 0x8000, B 0x4000, START 0x1000 */
} PadInput;

extern s32 D_800B6CF8;

void func_00275624(s32);

/* Returns whether the buttons (A_BUTTON | B_BUTTON | START_BUTTON) were pressed; when they were and
 * D_800B6CF8 is set, calls func_00275624(300). */
s32 func_002855E8(PadInput *pad) {
    if (!(pad->m_Pressed & 0xD000)) {
        return 0;
    }
    if (D_800B6CF8 == 0) {
        return 1;
    }
    func_00275624(300);
    return 1;
}
