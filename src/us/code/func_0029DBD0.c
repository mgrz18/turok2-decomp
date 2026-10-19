/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_27.c, func_802B9730_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern OSPiHandle_s *D_800B8980[];
s32 func_0029DBD0(OSPiHandle_s *arg0, u32 arg1, u32 arg2) {
    OSPiHandle_s *previous;
    s32 index;
    while (*(volatile u32 *)0xA4600010 & 3) {
    }
    index = arg0->domain;
    previous = D_800B8980[index];
    if (previous != arg0) {
        if (index == 0) {
            if (previous->latency != arg0->latency) {
                *(volatile u32 *)0xA4600014 = arg0->latency;
            }
            if (previous->pageSize != arg0->pageSize) {
                *(volatile u32 *)0xA460001C = arg0->pageSize;
            }
            if (previous->relDuration != arg0->relDuration) {
                *(volatile u32 *)0xA4600020 = arg0->relDuration;
            }
            if (previous->pulse != arg0->pulse) {
                *(volatile u32 *)0xA4600018 = arg0->pulse;
            }
        } else {
            if (previous->latency != arg0->latency) {
                *(volatile u32 *)0xA4600024 = arg0->latency;
            }
            if (previous->pageSize != arg0->pageSize) {
                *(volatile u32 *)0xA460002C = arg0->pageSize;
            }
            if (previous->relDuration != arg0->relDuration) {
                *(volatile u32 *)0xA4600030 = arg0->relDuration;
            }
            if (previous->pulse != arg0->pulse) {
                *(volatile u32 *)0xA4600028 = arg0->pulse;
            }
        }
        D_800B8980[index] = arg0;
    }
    *(volatile u32 *)(arg0->baseAddress | arg1 | 0xA0000000) = arg2;
    return 0;
}
