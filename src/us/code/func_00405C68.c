#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

s32 ScriptAnimator_SetScript(void *, f32 *);           /* extern */

#ifdef TARGET_PORT
#define MODULE (&gLevelScript) /* the N64 delta 0x3D4 is not the host's */
#else
#define MODULE ((LevelScriptState *) ((u8 *) &gLevelScript_bossMode - 0x3D4))
#endif

void Primagen_EnterMode7(CGameObjectInstance *arg0) {
    LevelScriptState *temp_s2;

    if (gLevelScript_bossMode.mode == 2) {
        gLevelScript_bossMode.armed = 1;
        gLevelScript_bossMode.scriptOn = 1;
        MODULE->bossState = 2;
    }
    temp_s2 = MODULE;
    if (MODULE->stage == 1) {
        arg0->m_AI.m_AnimSpeed = 1.0f;
    }
    if (MODULE->stage == 2) {
        arg0->m_AI.m_AnimSpeed = 1.2f;
    }
    if (MODULE->stage == 3) {
        arg0->m_AI.m_AnimSpeed = 1.3f;
    }
    if ((MODULE->stage == 1) && (temp_s2->spawnStep == 0)) {
        ScriptAnimator_SetScript(&MODULE->anims[3], sPrimagenMode7PartScript);
        ScriptAnimator_SetScript(&MODULE->anims[4], sPrimagenMode7PartScript);
        ScriptAnimator_SetScript(&MODULE->anims[6], sPrimagenMode7PartScript);
        ScriptAnimator_SetScript(&MODULE->anims[5], sPrimagenMode7PartScript);
        temp_s2->spawnStep = (s32) (temp_s2->spawnStep + 1);
    }
    if ((gLevelScript.stage == 2) && (gLevelScript.spawnStep == 1)) {
        ScriptAnimator_SetScript(&gLevelScript.anims[0], sPrimagenMode7PartScript);
        ScriptAnimator_SetScript(&gLevelScript.anims[2], sPrimagenMode7PartScript);
        gLevelScript.spawnStep = (s32) (gLevelScript.spawnStep + 1);
    }
    if ((gLevelScript.stage == 3) && (gLevelScript.spawnStep == 2)) {
        ScriptAnimator_SetScript(&gLevelScript.anims[1], sPrimagenMode7PartScript);
        gLevelScript.spawnStep = (s32) (gLevelScript.spawnStep + 1);
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
