/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_20.c, func_802BA210_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0029A5E0(void);
extern void func_0029ACE0();
extern __OSViContext_func_802BA910_de D_800B7A60[2];
extern __OSViContext_func_802BA910_de *D_800B7AC0;
extern __OSViContext_func_802BA910_de *D_800B7AC4;
extern Modep D_800B7AF0;
extern Modep D_800B7B40;
extern Modep D_800B7B90;
extern s32 D_80000300;
extern void *func_00266C5C(void *destination, s32 value, u32 size);
void func_0029A5E0(void)
{
    __OSViContext_func_802BA910_de *contexts = D_800B7A60;
    s32 tv_type;
    func_00266C5C(contexts, 0, sizeof(D_800B7A60));
    tv_type = D_80000300;
    D_800B7AC0 = &contexts[0];
    D_800B7AC4 = &contexts[1];
    contexts[1].retraceCount = 1;
    contexts[0].retraceCount = 1;
    contexts[1].framep = (void *)0x80000000U;
    contexts[0].framep = (void *)0x80000000U;
    if (tv_type == 0) {
        contexts[1].modep = &D_800B7B40;
    } else if (tv_type == 2) {
        contexts[1].modep = &D_800B7B90;
    } else {
        contexts[1].modep = &D_800B7AF0;
    }
    D_800B7AC4->state = 0x20;
    D_800B7AC4->control = D_800B7AC4->modep->comRegs.ctrl;
    while ((*(volatile u32 *)(0xA4400010U)) > 10) {
    }
    (*(volatile u32 *)(0xA4400000U) = (0));
    func_0029ACE0();
}
