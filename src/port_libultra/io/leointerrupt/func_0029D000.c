/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_18.c, func_802BCC90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern void __osLeoAbnormalResume();
extern void __osLeoResume();
void __osLeoAbnormalResume(void) {
    __OSTranxInfo *info;
    u32 status;
    status = *(volatile u32 *)0xA4600010;
    info = &__osDiskHandle->transferInfo;
    while (status & 3) {
        status = *(volatile u32 *)0xA4600010;
    }
    *(volatile u32 *)0xA5000510 = info->bmCtlShadow | 0x10000000;
    status = *(volatile u32 *)0xA4600010;
    while (status & 3) {
        status = *(volatile u32 *)0xA4600010;
    }
    *(volatile u32 *)0xA5000510 = info->bmCtlShadow;
    __osLeoResume();
    {
        volatile u32 *statusAddr = (volatile u32 *)0xA4600010;
        s32 mask = 0x100401;
        s32 *flagsAddr = &__OSGlobalIntMask;
        s32 flags = *flagsAddr;
        *statusAddr = 2;
        *flagsAddr = flags | mask;
    }
}
void __osLeoResume(void);
