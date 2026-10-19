/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_25.c, func_802B9590_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_0029DA30(struct OSPiHandle_s *handle, u32 address, u32 *value);
extern OSPiHandle_s *D_800B8980[];
extern s32 D_800AA9B0;
extern s32 D_800AA9B4;
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
s32 func_0029DA30(OSPiHandle_s *arg0, u32 arg1, u32 *arg2) {
    OSPiHandle_s *previous;
    s32 index;
    if (arg2 == 0) {
        func_0029B020(&D_800AA9B0, &D_800AA9B4, 0x2B);
    }
    while ((*(volatile u32 *)(0xA4600010U)) & 3) {
    }
    index = arg0->domain;
    previous = D_800B8980[index];
    if (previous != arg0) {
        if (index == 0) {
            if (previous->latency != arg0->latency) {
                (*(volatile u32 *)(0xA4600014U) = (arg0->latency));
            }
            if (previous->pageSize != arg0->pageSize) {
                (*(volatile u32 *)(0xA460001CU) = (arg0->pageSize));
            }
            if (previous->relDuration != arg0->relDuration) {
                (*(volatile u32 *)(0xA4600020U) = (arg0->relDuration));
            }
            if (previous->pulse != arg0->pulse) {
                (*(volatile u32 *)(0xA4600018U) = (arg0->pulse));
            }
        } else {
            if (previous->latency != arg0->latency) {
                (*(volatile u32 *)(0xA4600024U) = (arg0->latency));
            }
            if (previous->pageSize != arg0->pageSize) {
                (*(volatile u32 *)(0xA460002CU) = (arg0->pageSize));
            }
            if (previous->relDuration != arg0->relDuration) {
                (*(volatile u32 *)(0xA4600030U) = (arg0->relDuration));
            }
            if (previous->pulse != arg0->pulse) {
                (*(volatile u32 *)(0xA4600028U) = (arg0->pulse));
            }
        }
        D_800B8980[index] = arg0;
    }
    *arg2 = (*(volatile u32 *)(arg0->baseAddress | arg1 | 0xA0000000U));
    return 0;
}
