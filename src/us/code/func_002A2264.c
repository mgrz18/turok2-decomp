/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002014A4.c, func_002014A4, with Turok 2's layouts. */

#include "common.h"

extern u8 D_80128880;   /* debugger: stopped */
extern u8 D_80128882;   /* debugger: resume allowed */
extern u32 D_801286D8;
extern u32 func_002A1384(u32 address);
extern void func_002A1350(u32 address, u32 value);
extern void func_002A13BC(u32 n);
extern void func_002A1DEC(u32 mode);
extern void func_002A2008(void);
extern u32 func_002A31E4(void);
extern void func_002A31F0(u32 sr);

/* The debugger host's command loop: wait for commands in the mailbox at 0xB1FFFFF0 and run them
 * until told to continue. */
void func_002A2264(void) {
    s32 cont;
    u32 cmd;

    do {
        cont = 1;
        while ((cmd = func_002A1384(0xB1FFFFF0)) == 0) {
            func_002A13BC(1000);
        }
        if (cmd == 2) {
            func_002A1350(0xB1FFFFFC, 0);
            func_002A13BC(1000);
        } else {
            func_002A1350(0xB1FFFFFC, 0x101);
            while (func_002A1384(0xB1FFFFF0) == cmd) {
                func_002A13BC(500);
            }
            if (func_002A1384(0xB1FFFFF0) == 2) {
                func_002A1350(0xB1FFFFFC, 0);
            } else {
                switch (cmd) {
                case 0x10:
                    break;
                case 0x11:
                    cont = 0;
                    break;
                case 0x12:
                    D_801286D8 &= ~1;
                    D_80128880 = 0;
                    D_80128882 = 0;
                    func_002A31F0(func_002A31E4() & ~1);
                    break;
                case 0x13:
                    D_80128882 = 0;
                    D_80128880 = 0;
                    break;
                case 0x14:
                    D_80128882 = 0xFF;
                    break;
                case 0x15:
                    D_80128882 = 0;
                    break;
                case 0x16:
                    func_002A1350(0xB1FE0000, (D_80128882 << 24) | (D_80128880 << 16));
                    break;
                case 0x17:
                    func_002A1DEC(2);
                    break;
                case 0x18:
                    func_002A1DEC(1);
                    break;
                case 0x19:
                    func_002A2008();
                    break;
                }
                func_002A1350(0xB1FFFFFC, 0);
                while (func_002A1384(0xB1FFFFF0) == 1) {
                    func_002A13BC(500);
                }
            }
        }
    } while (cont != 0 || D_80128882 == 0);
    func_002A1350(0xB1FFFFF0, 0);
}
