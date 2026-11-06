/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_23.c, func_802B93B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern u32 osVirtualToPhysical(void *addr);
s32 osEPiRawStartDma(OSPiHandle_s *pihandle, s32 direction, u32 devAddr, void *dramAddr, u32 size)
{
    u32 stat;
    u32 domain;
    stat = *(volatile u32 *)0xA4600010;
    while (stat & 3)
        stat = *(volatile u32 *)0xA4600010;
    domain = pihandle->domain;
    if (__osCurrentHandle[domain] != pihandle) {
        OSPiHandle_s *cHandle = __osCurrentHandle[domain];
        if (domain == 0) {
            if (cHandle->latency != pihandle->latency) (*(volatile u32 *)(0xA4600014) = (pihandle->latency));
            if (cHandle->pageSize != pihandle->pageSize) (*(volatile u32 *)(0xA460001C) = (pihandle->pageSize));
            if (cHandle->relDuration != pihandle->relDuration) (*(volatile u32 *)(0xA4600020) = (pihandle->relDuration));
            if (cHandle->pulse != pihandle->pulse) (*(volatile u32 *)(0xA4600018) = (pihandle->pulse));
        } else {
            if (cHandle->latency != pihandle->latency) (*(volatile u32 *)(0xA4600024) = (pihandle->latency));
            if (cHandle->pageSize != pihandle->pageSize) (*(volatile u32 *)(0xA460002C) = (pihandle->pageSize));
            if (cHandle->relDuration != pihandle->relDuration) (*(volatile u32 *)(0xA4600030) = (pihandle->relDuration));
            if (cHandle->pulse != pihandle->pulse) (*(volatile u32 *)(0xA4600028) = (pihandle->pulse));
        }
        __osCurrentHandle[domain] = pihandle;
    }
    (*(volatile u32 *)(0xA4600000) = (osVirtualToPhysical(dramAddr)));
    (*(volatile u32 *)(0xA4600004) = ((pihandle->baseAddress | devAddr) & 0x1FFFFFFF));
    switch (direction) {
        case 0:
            (*(volatile u32 *)(0xA460000C) = (size - 1));
            break;
        case 1:
            (*(volatile u32 *)(0xA4600008) = (size - 1));
            break;
        default:
            return -1;
    }
    return 0;
}
