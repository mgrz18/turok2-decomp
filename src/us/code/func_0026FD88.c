#include "common.h"
#include "m2c_macros.h"

s32 func_002949A4(s32, s32, s32, s32);
s32 func_002985C0(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 func_0029BF80(s32);
void func_0029E3B0(void);

M2C_UNK func_0026FEA0();                            /* extern */
M2C_UNK func_0027592C();                            /* extern */
M2C_UNK func_00275B7C();                            /* extern */
M2C_UNK func_00275BA0();                            /* extern */
M2C_UNK func_0028B3E8(M2C_UNK *, void *);           /* extern */
extern s32 D_800B2668;
extern M2C_UNK D_800F27C0;
extern M2C_UNK D_800F2820;
extern u16 D_800F283A;
extern M2C_UNK D_8011AD90;

s32 func_0026FD88(void *arg0) {
    s32 sp10;
    M2C_UNK sp14;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    func_0026FEA0();
    temp_s0 = func_0029BF80(M2C_FIELD(arg0, s32 *, 0));
    M2C_FIELD(arg0, u16 *, 4) = (u16) D_800F283A;
    func_00275B7C();
    temp_s0_2 = func_002949A4(*(((__typeof__(&D_800F27C0))((s8 *)&D_800F27C0 + (D_800B2668 * 4)))), (s32) &sp10, temp_s0, (s32) (s16) M2C_FIELD(arg0, u16 *, 4));
    func_00275BA0();
    if (sp10 == 0) {
        return 0;
    }
    temp_v0 = D_800B2668 * 4;
    M2C_FIELD(arg0, s32 *, 0x48) = (s32) *(((__typeof__(&D_800F27C0))((s8 *)&D_800F27C0 + temp_v0)));
    M2C_FIELD(arg0, s32 *, 0x4C) = (s32) (((s32) (temp_s0_2 - *(((__typeof__(&D_800F27C0))((s8 *)&D_800F27C0 + temp_v0)))) >> 3) * 8);
    func_0029E3B0();
    func_0028B3E8(&D_8011AD90, arg0 + 8);
    D_800B2668 ^= 1;
    func_0027592C();
    osRecvMesg((s32) &D_800F2820, (s32) &sp14, 1);
    func_002985C0(M2C_FIELD(arg0, s32 *, 0), (s16) M2C_FIELD(arg0, u16 *, 4) * 4);
    return 1;
}
