/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_18.c, func_802BCC90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_800B8908;
extern void func_0029D000();
extern void __osLeoResume();
extern char *D_80123AD4;
void func_0029D000(void) {
    DiskTransferInfo *temp_a2;
    u32 status;
    status = *(volatile u32 *)0xA4600010;
    temp_a2 = (DiskTransferInfo *)(D_80123AD4 + 0x14);
    while (status & 3) {
        status = *(volatile u32 *)0xA4600010;
    }
    *(volatile u32 *)0xA5000510 = temp_a2->bmControl | 0x10000000;
    status = *(volatile u32 *)0xA4600010;
    while (status & 3) {
        status = *(volatile u32 *)0xA4600010;
    }
    *(volatile u32 *)0xA5000510 = temp_a2->bmControl;
    __osLeoResume();
    {
        volatile u32 *statusAddr = (volatile u32 *)0xA4600010;
        s32 mask = 0x100401;
        s32 *flagsAddr = &D_800B8908;
        s32 flags = *flagsAddr;
        *statusAddr = 2;
        *flagsAddr = flags | mask;
    }
}
void __osLeoResume(void);
