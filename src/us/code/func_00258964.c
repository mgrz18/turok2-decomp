#include "common.h"
#include "m2c_macros.h"
#include "globals/player.h"

s32 WeaponActor_Update(s32, s32);
s32 WeaponActor_Draw(s32, s32, s32);

#ifdef TARGET_PORT
#include "structs/game_object.h"

void WeaponActor_SwitchToRequestedInline(CGameObjectInstance *weapon, CAI *state);

/* Host body: the AI state at host offsets (the N64 body writes CAI 0x2C/0xF0/0xF4/0x10C+). */
void WeaponActor_InitAI(CGameObjectInstance *weapon, CAI *ai) {
    ai->m_pModeTable = sWeaponActorModeTable;
    ai->m_pfStep = (void *)WeaponActor_Update;
    ai->m_pfDraw = (void *)WeaponActor_Draw;
    ai->m_VarS32[0] = 0;
    ai->m_VarS32[1] = 0;
    ai->m_VarS32[3] = 0;
    ai->m_VarS32[5] = 0;
    ai->m_VarS32[6] = 0;
    ai->m_VarS32[7] = 0;
    ai->m_VarS32[8] = 0;
    ai->m_VarS32[9] = 0;
    ai->m_VarF32[2] = 1.0f;
    WeaponActor_SwitchToRequestedInline(weapon, ai);
}
#else
s32 WeaponActor_SwitchToRequestedInline(s32, s32);

void WeaponActor_InitAI(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, CAIModeInfo **, 0x2C) = sWeaponActorModeTable;
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
    WeaponActor_SwitchToRequestedInline(arg0, (sptr) arg1);
}
#endif
