/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/CISet__GetBlockAndSize.c, CISet__GetBlockAndSize, with Turok 2's layouts. */

#include "common.h"

extern void HostIo_WriteWord(u32 address, u32 value);
extern s32 HostIo_ReadWord(u32 address);
extern void HostIo_Delay(s32 count);

/* Posts a command to the host through the 0xB1FFFFxx registers and waits until it is done. */
s32 HostIo_Call(s32 cmd, s32 arg0, s32 arg1, s32 arg2) {
    HostIo_WriteWord(0xB1FFFFE4, arg0);
    HostIo_WriteWord(0xB1FFFFE8, arg1);
    HostIo_WriteWord(0xB1FFFFEC, arg2);
    HostIo_WriteWord(0xB1FFFFE0, cmd);
    do {
        HostIo_Delay(1000);
        if (HostIo_ReadWord(0xB1FFFFF0) != 0) {
            __asm__ volatile("break 1");
        }
    } while (HostIo_ReadWord(0xB1FFFFE0) != 0);
    return HostIo_ReadWord(0xB1FFFFDC);
}
