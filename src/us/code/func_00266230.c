#include "common.h"
#include "structs/game_object.h"

void func_00220260(void *, s32, s32);               /* extern */
void func_0026E110(s32, s32, s32);                  /* extern */
extern u8 D_800F7078[];

void func_00266230(s32 arg0, CAI *arg1) {
    func_00220260(D_800F7078, arg0, 0);
    func_0026E110(arg0, 0x200000, arg0);
    arg1->m_VarBuffer[2] = 0;
}
