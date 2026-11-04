#include "common.h"
#include "structs/player.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_0042E29C(s32);

extern s32 D_800ACDE4;
extern s32 D_800AE340;
extern CPlayer *D_800F1FD0;

void func_00257784(void) {
    if (D_800F1FD0->m_Data.m_Lives == 1) {
        func_0042E29C((s32) D_800F1FD0);
    } else {
        func_0024E5F0((s32) D_800F1FD0, (s32) D_800F1FD0, 0x1F);
    }
    D_800ACDE4 = 1;
    D_800AE340 = 0;
}
