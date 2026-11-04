#include "common.h"

s32 func_00412438(s32);                         /* extern */
s32 func_0042C418(s32 *);                   /* extern */
s32 func_0042C46C(s32 *);                   /* extern */
s32 func_0042C700(s32 *, s32, s32);     /* extern */
s32 func_0042E5AC(s32, s32, s32);       /* extern */
s32 func_0042FBA4(s32, s32);                /* extern */
extern s32 D_800F7078;

void func_00288DD0(s32 arg0, s32 arg1) {
    func_0042FBA4(arg0 + 0x22C00, 1);
    func_0042E5AC(arg0 + 0x22BC0, 1, 0);
    func_0042C700(&D_800F7078, ~func_00412438(arg1), 0);
    func_0042C418(&D_800F7078);
    func_0042C46C(&D_800F7078);
}
