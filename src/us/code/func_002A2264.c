/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002014A4.c, func_002014A4, with Turok 2's layouts. */

#include "common.h"
#include "globals/hostio.h"

extern u32 D_801286D8;
extern u32 HostIo_ReadWord(u32 address);
extern void HostIo_WriteWord(u32 address, u32 value);
extern void HostIo_Delay(u32 n);
extern void HostIo_MemoryTransfer(u32 mode);
extern void HostIo_CopyMailboxBlock(void);
extern u32 func_002A31E4(void);
extern void func_002A31F0(u32 sr);

/* The debugger host's command loop: wait for commands in the mailbox at 0xB1FFFFF0 and run them
 * until told to continue. */
void HostIo_CommandLoop(void) {
    s32 cont;
    u32 cmd;

    do {
        cont = 1;
        while ((cmd = HostIo_ReadWord(0xB1FFFFF0)) == 0) {
            HostIo_Delay(1000);
        }
        if (cmd == 2) {
            HostIo_WriteWord(0xB1FFFFFC, 0);
            HostIo_Delay(1000);
        } else {
            HostIo_WriteWord(0xB1FFFFFC, 0x101);
            while (HostIo_ReadWord(0xB1FFFFF0) == cmd) {
                HostIo_Delay(500);
            }
            if (HostIo_ReadWord(0xB1FFFFF0) == 2) {
                HostIo_WriteWord(0xB1FFFFFC, 0);
            } else {
                switch (cmd) {
                case 0x10:
                    break;
                case 0x11:
                    cont = 0;
                    break;
                case 0x12:
                    D_801286D8 &= ~1;
                    sHostIoStopped = 0;
                    sHostIoResumeAllowed = 0;
                    func_002A31F0(func_002A31E4() & ~1);
                    break;
                case 0x13:
                    sHostIoResumeAllowed = 0;
                    sHostIoStopped = 0;
                    break;
                case 0x14:
                    sHostIoResumeAllowed = 0xFF;
                    break;
                case 0x15:
                    sHostIoResumeAllowed = 0;
                    break;
                case 0x16:
                    HostIo_WriteWord(0xB1FE0000, (sHostIoResumeAllowed << 24) | (sHostIoStopped << 16));
                    break;
                case 0x17:
                    HostIo_MemoryTransfer(2);
                    break;
                case 0x18:
                    HostIo_MemoryTransfer(1);
                    break;
                case 0x19:
                    HostIo_CopyMailboxBlock();
                    break;
                }
                HostIo_WriteWord(0xB1FFFFFC, 0);
                while (HostIo_ReadWord(0xB1FFFFF0) == 1) {
                    HostIo_Delay(500);
                }
            }
        }
    } while (cont != 0 || sHostIoResumeAllowed == 0);
    HostIo_WriteWord(0xB1FFFFF0, 0);
}
