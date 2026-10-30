/* NON-MATCHING: 101 of 112 words differ: m2c draft, not worked on (time) */
#include "common.h"
#include "m2c_macros.h"

s32 func_0041D890(s32);
s32 func_0041DA28(s32);
s32 func_00426480(s32);

extern M2C_UNK D_800C2018;
extern M2C_UNK D_800C2020;
extern M2C_UNK D_800C2024;

void func_00416644();
s32 func_0042655C(s32);                             /* extern */
s32 func_00426594(s32);                             /* extern */
s32 func_00426BD8(s32);                             /* extern */
s32 func_00426E78(s32, s32, M2C_UNK);               /* extern */
extern M2C_UNK D_0043BC00;
extern M2C_UNK D_0043BC48;
extern M2C_UNK D_0043BEF4;
extern M2C_UNK D_0043C05C;
extern s32 D_800F70A8;
extern M2C_UNK D_80119E2C;
extern s32 D_80130940;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_801309A0;
extern s32 D_801309B4;
extern s32 D_801309BC;

void func_0041EA84(void *arg0) {
    s32 sp18;
    M2C_UNK *var_a0_2;
    M2C_UNK *var_a1;
    s32 var_a0;
    s32 var_a3;
    void *temp_v0;

    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 0;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    sp18 = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4);
    temp_v0 = M2C_FIELD(arg0, void **, 0x1C);
    if (D_800C2020 != 0) {
        D_801309BC = 1;
        func_00416644(M2C_FIELD(temp_v0, s32 *, 0x51C) + 0x53C, &D_0043BEF4, temp_v0, M2C_FIELD(temp_v0, s32 *, 0xA50), 0);
        D_80130940 = 0;
        return;
    }
    func_00426480((s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4));
    if (func_0042655C(sp18) != 0) {
        D_801309BC = 1;
        if (func_00426594(sp18) != 0) {
            var_a1 = &D_0043BC48;
            var_a0 = M2C_FIELD(temp_v0, s32 *, 0x51C);
            goto block_11;
        }
        if (func_00426BD8(sp18) != 0) {
            var_a0_2 = &D_80119E2C;
            var_a1 = &D_0043C05C;
            var_a3 = M2C_FIELD(temp_v0, s32 *, 0xA50);
        } else {
            var_a1 = &D_0043BC00;
            var_a0 = M2C_FIELD(temp_v0, s32 *, 0x51C);
block_11:
            var_a3 = M2C_FIELD(temp_v0, s32 *, 0xA50);
            var_a0_2 = var_a0 + 0x53C;
        }
        func_00416644(var_a0_2, var_a1, temp_v0, var_a3);
        return;
    }
    func_0041D890((s32) temp_v0);
    M2C_FIELD((D_800F70A8 + D_800C2018), s32 *, 0x684) = func_00426E78(D_800C2018, D_800F70A8 + 0x684, 0xB);
    func_0041DA28((s32) arg0);
}
