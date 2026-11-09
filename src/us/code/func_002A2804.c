/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* Boundary fixed in versions/cuts.us.txt: splat's HostIo_ThreadMain (0x208 bytes) is two functions: the I/O thread HostIo_ThreadMain
 * (0x1B4 bytes, an endless loop with no epilogue) and PCinit (0x54 bytes) right after it,
 * which starts at 0x2A2804 with its own `addiu $sp, $sp, -0x18` prologue. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002018A0.c, func_002018A0,
 * and src/game/func_00201A74.c, func_00201A74, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "structs/hostio.h"
#include "protos/ultra.h"

extern u32 HostIo_ReadWord(u32 address);
extern void HostIo_WriteWord(u32 address, u32 value);
extern void HostIo_WriteBlockDma(u32 address, u8 *source, u32 count);
extern void HostIo_ReadBlockDma(u32 address, u8 *dest, u32 count);
extern s32 HostIo_DisableInt(void);
extern void HostIo_RestoreInt(s32 saved);
extern s32 HostIo_Call(s32 cmd, s32 arg0, s32 arg1, s32 arg2);

s32 PCinit(void) {
    s32 saved;
    s32 result;

    saved = HostIo_DisableInt();
    sHostIoState = 1;
    result = HostIo_Call(0x101, 0, 0, 0);
    HostIo_RestoreInt(saved);
    return result;
}
