#include "common.h"
#include "m2c_macros.h"

s32 WeaponActor_Update(s32, s32);
s32 WeaponActor_SwitchToRequestedInline(s32, s32);
s32 WeaponActor_Draw(s32, s32, s32);

extern M2C_UNK sWeaponActorModeTable;

void WeaponActor_InitAI(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &sWeaponActorModeTable;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF0) = WeaponActor_Update;
    M2C_FIELD(arg1, s32 (**)(s32, s32, s32), 0xF4) = WeaponActor_Draw;
    M2C_FIELD(arg1, s32 *, 0x10C) = 0;
    M2C_FIELD(arg1, s32 *, 0x110) = 0;
    M2C_FIELD(arg1, s32 *, 0x118) = 0;
    M2C_FIELD(arg1, s32 *, 0x120) = 0;
    M2C_FIELD(arg1, s32 *, 0x124) = 0;
    M2C_FIELD(arg1, s32 *, 0x128) = 0;
    M2C_FIELD(arg1, s32 *, 0x12C) = 0;
    M2C_FIELD(arg1, s32 *, 0x130) = 0;
    M2C_FIELD(arg1, f32 *, 0x114) = 1.0f;
    WeaponActor_SwitchToRequestedInline(arg0, (s32) arg1);
}
