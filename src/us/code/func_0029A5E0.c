/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_20.c, func_802BA210_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern void __osViInit(void);
extern void __osViSwapContext();
extern __OSViContext __osViContexts[2];
extern __OSViContext *__osViCurr;
extern __OSViContext *__osViNext;
extern OSViMode osViModeNtscLan1;
extern OSViMode osViModePalLan1;
extern OSViMode osViModeMpalLan1;
extern void *memset(void *destination, s32 value, u32 size);
void __osViInit(void)
{
    __OSViContext *contexts = __osViContexts;
    s32 tv_type;
    memset(contexts, 0, sizeof(__osViContexts));
    tv_type = osTvType;
    __osViCurr = &contexts[0];
    __osViNext = &contexts[1];
    contexts[1].retraceCount = 1;
    contexts[0].retraceCount = 1;
    contexts[1].framep = (void *)0x80000000U;
    contexts[0].framep = (void *)0x80000000U;
    if (tv_type == 0) {
        contexts[1].modep = &osViModePalLan1;
    } else if (tv_type == 2) {
        contexts[1].modep = &osViModeMpalLan1;
    } else {
        contexts[1].modep = &osViModeNtscLan1;
    }
    __osViNext->state = 0x20;
    __osViNext->control = __osViNext->modep->comRegs.ctrl;
    while ((*(volatile u32 *)(0xA4400010U)) > 10) {
    }
    (*(volatile u32 *)(0xA4400000U) = (0));
    __osViSwapContext();
}
