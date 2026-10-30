/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002017E8.c, func_002017E8, with Turok 2's layouts. */

#include "common.h"

extern void func_002A1350(u32 address, u32 value);
extern s32 func_002A1384(u32 address);
extern void func_002A13BC(s32 count);

/* Posts a command to the host through the 0xB1FFFFxx registers and waits until it is done. */
s32 func_002A259C(s32 cmd, s32 arg0, s32 arg1, s32 arg2) {
    func_002A1350(0xB1FFFFE4, arg0);
    func_002A1350(0xB1FFFFE8, arg1);
    func_002A1350(0xB1FFFFEC, arg2);
    func_002A1350(0xB1FFFFE0, cmd);
    do {
        func_002A13BC(1000);
        if (func_002A1384(0xB1FFFFF0) != 0) {
            __asm__ volatile("break 1");
        }
    } while (func_002A1384(0xB1FFFFE0) != 0);
    return func_002A1384(0xB1FFFFDC);
}
