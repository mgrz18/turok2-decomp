/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_34.c, func_802BA910_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800AA8D8;
extern double D_800AA8D0;
extern void func_0029ACE0();
extern __OSViContext_func_802BA910_de *D_800B7AC0;
extern __OSViContext_func_802BA910_de *D_800B7AC4;
extern u32 func_0029BF80(void *addr);
void func_0029ACE0(void)
{
    register Modep *vm;
    register __OSViContext_func_802BA910_de *vc;
    u32 origin;
    u32 hStart;
    u32 nomValue;
    u32 field;
    f64 nom;
    f32 scale;
    u32 yscale;
    u32 *yscalep;
    __OSViContext_func_802BA910_de **curr;
    field = 0;
    vc = D_800B7AC4;
    vm = vc->modep;
    field = (*(volatile u32 *)(0xA4400010)) & 1;
    origin = func_0029BF80(vc->framep) + (vm->fldRegs[field].origin);
    if (vc->state & 2) {
        vc->x.scale |= (vm->comRegs.xScale & ~0xFFF);
    } else {
        vc->x.scale = vm->comRegs.xScale;
    }
    if (vc->state & 4) {
        nomValue = vm->fldRegs[field].yScale & 0xFFF;
        yscalep = &vc->y.scale;
        nom = (s32)nomValue;
        if ((s32)nomValue < 0) {
            nom += D_800AA8D0;
        }
        scale = vc->y.factor * (f32)nom;
        if (scale >= D_800AA8D8) {
            goto large;
        }
        yscale = (s32)scale;
        goto converted;
    large:
        yscale = (s32)(scale - D_800AA8D8) | 0x80000000;
    converted:
        *yscalep = yscale;
        vc->y.scale |= vm->fldRegs[field].yScale & ~0xFFF;
    } else {
        vc->y.scale = vm->fldRegs[field].yScale;
    }
    hStart = vm->comRegs.hStart;
    if (vc->state & 0x20) {
        hStart = 0;
    }
    if (vc->state & 0x40) {
        vc->y.scale = 0;
        origin = func_0029BF80(vc->framep);
    }
    if (vc->state & 0x80) {
        vc->y.scale = (vc->y.offset << 16) & (0x3FF << 16);
        origin = func_0029BF80(vc->framep);
    }
    (*(volatile u32 *)(0xA4400004) = (origin));
    (*(volatile u32 *)(0xA4400008) = (vm->comRegs.width));
    (*(volatile u32 *)(0xA4400014) = (vm->comRegs.burst));
    (*(volatile u32 *)(0xA4400018) = (vm->comRegs.vSync));
    (*(volatile u32 *)(0xA440001C) = (vm->comRegs.hSync));
    (*(volatile u32 *)(0xA4400020) = (vm->comRegs.leap));
    (*(volatile u32 *)(0xA4400024) = (hStart));
    (*(volatile u32 *)(0xA4400028) = (vm->fldRegs[field].vStart));
    (*(volatile u32 *)(0xA440002C) = (vm->fldRegs[field].vBurst));
    (*(volatile u32 *)(0xA440000C) = (vm->fldRegs[field].vIntr));
    (*(volatile u32 *)(0xA4400030) = (vc->x.scale));
    (*(volatile u32 *)(0xA4400034) = (vc->y.scale));
    (*(volatile u32 *)(0xA4400000) = (vc->control));
    curr = &D_800B7AC0;
    D_800B7AC4 = *curr;
    *curr = vc;
    *D_800B7AC4 = **curr;
}
