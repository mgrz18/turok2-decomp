#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00236314(void *, M2C_UNK *);           /* extern */
extern M2C_UNK D_00431B54;
extern f32 D_800C0488;
extern f32 D_800C048C;
extern f32 D_800C0490;
extern M2C_UNK D_8012F608;
extern M2C_UNK D_8012F9DC;

void func_00405C68(void *arg0) {
    void *temp_s2;

    if (M2C_FIELD(&D_8012F9DC, s32 *, 0) == 2) {
        M2C_FIELD(&D_8012F9DC, s8 *, 4) = 1;
        M2C_FIELD(&D_8012F9DC, s8 *, 5) = 1;
        M2C_FIELD(&D_8012F9DC, s32 *, -4) = 2;
    }
    temp_s2 = ((__typeof__(&D_8012F9DC))((s8 *)&D_8012F9DC - 0x3D4));
    if (M2C_FIELD(&D_8012F9DC, s32 *, -0x3D4) == 1) {
        M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) D_800C0488;
    }
    if (M2C_FIELD(&D_8012F9DC, s32 *, -0x3D4) == 2) {
        M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) D_800C048C;
    }
    if (M2C_FIELD(&D_8012F9DC, s32 *, -0x3D4) == 3) {
        M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) D_800C0490;
    }
    if ((M2C_FIELD(&D_8012F9DC, s32 *, -0x3D4) == 1) && (M2C_FIELD(temp_s2, s32 *, 0x18) == 0)) {
        func_00236314(((__typeof__(&D_8012F9DC))((s8 *)&D_8012F9DC - 0x284)), &D_00431B54);
        func_00236314(((__typeof__(&D_8012F9DC))((s8 *)&D_8012F9DC - 0x220)), &D_00431B54);
        func_00236314(((__typeof__(&D_8012F9DC))((s8 *)&D_8012F9DC - 0x158)), &D_00431B54);
        func_00236314(((__typeof__(&D_8012F9DC))((s8 *)&D_8012F9DC - 0x1BC)), &D_00431B54);
        M2C_FIELD(temp_s2, s32 *, 0x18) = (s32) (M2C_FIELD(temp_s2, s32 *, 0x18) + 1);
    }
    if ((M2C_FIELD(&D_8012F608, s32 *, 0) == 2) && (M2C_FIELD(&D_8012F608, s32 *, 0x18) == 1)) {
        func_00236314(((__typeof__(&D_8012F608))((s8 *)&D_8012F608 + 0x24)), &D_00431B54);
        func_00236314(((__typeof__(&D_8012F608))((s8 *)&D_8012F608 + 0xEC)), &D_00431B54);
        M2C_FIELD(&D_8012F608, s32 *, 0x18) = (s32) (M2C_FIELD(&D_8012F608, s32 *, 0x18) + 1);
    }
    if ((M2C_FIELD(&D_8012F608, s32 *, 0) == 3) && (M2C_FIELD(&D_8012F608, s32 *, 0x18) == 2)) {
        func_00236314(((__typeof__(&D_8012F608))((s8 *)&D_8012F608 + 0x88)), &D_00431B54);
        M2C_FIELD(&D_8012F608, s32 *, 0x18) = (s32) (M2C_FIELD(&D_8012F608, s32 *, 0x18) + 1);
    }
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2000);
}
