#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

M2C_UNK ScriptAnimator_SetScript(void *, M2C_UNK *);           /* extern */
extern M2C_UNK sPrimagenMode7PartScript_2; /* local type: f32[] in globals/boss.h, whose structs/boss.h clashes with this file's M2C_UNK gLevelScript */
extern f32 D_800C0488;
extern f32 D_800C048C;
extern f32 D_800C0490;
extern M2C_UNK gLevelScript; /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */
extern M2C_UNK gLevelScript_bossMode;

void Primagen_EnterMode7_2(CGameObjectInstance *arg0) {
    void *temp_s2;

    if (M2C_FIELD(&gLevelScript_bossMode, s32 *, 0) == 2) {
        M2C_FIELD(&gLevelScript_bossMode, s8 *, 4) = 1;
        M2C_FIELD(&gLevelScript_bossMode, s8 *, 5) = 1;
        M2C_FIELD(&gLevelScript_bossMode, s32 *, -4) = 2;
    }
    temp_s2 = ((__typeof__(&gLevelScript_bossMode))((s8 *)&gLevelScript_bossMode - 0x3D4));
    if (M2C_FIELD(&gLevelScript_bossMode, s32 *, -0x3D4) == 1) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C0488;
    }
    if (M2C_FIELD(&gLevelScript_bossMode, s32 *, -0x3D4) == 2) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C048C;
    }
    if (M2C_FIELD(&gLevelScript_bossMode, s32 *, -0x3D4) == 3) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C0490;
    }
    if ((M2C_FIELD(&gLevelScript_bossMode, s32 *, -0x3D4) == 1) && (M2C_FIELD(temp_s2, s32 *, 0x18) == 0)) {
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript_bossMode))((s8 *)&gLevelScript_bossMode - 0x284)), &sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript_bossMode))((s8 *)&gLevelScript_bossMode - 0x220)), &sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript_bossMode))((s8 *)&gLevelScript_bossMode - 0x158)), &sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript_bossMode))((s8 *)&gLevelScript_bossMode - 0x1BC)), &sPrimagenMode7PartScript_2);
        M2C_FIELD(temp_s2, s32 *, 0x18) = (s32) (M2C_FIELD(temp_s2, s32 *, 0x18) + 1);
    }
    if ((M2C_FIELD(&gLevelScript, s32 *, 0) == 2) && (M2C_FIELD(&gLevelScript, s32 *, 0x18) == 1)) {
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript))((s8 *)&gLevelScript + 0x24)), &sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript))((s8 *)&gLevelScript + 0xEC)), &sPrimagenMode7PartScript_2);
        M2C_FIELD(&gLevelScript, s32 *, 0x18) = (s32) (M2C_FIELD(&gLevelScript, s32 *, 0x18) + 1);
    }
    if ((M2C_FIELD(&gLevelScript, s32 *, 0) == 3) && (M2C_FIELD(&gLevelScript, s32 *, 0x18) == 2)) {
        ScriptAnimator_SetScript(((__typeof__(&gLevelScript))((s8 *)&gLevelScript + 0x88)), &sPrimagenMode7PartScript_2);
        M2C_FIELD(&gLevelScript, s32 *, 0x18) = (s32) (M2C_FIELD(&gLevelScript, s32 *, 0x18) + 1);
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
