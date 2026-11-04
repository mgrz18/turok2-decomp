#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

s32 func_00236314(void *, s32 *);           /* extern */
extern s32 D_00431B54;
extern f32 D_800C0488;
extern f32 D_800C048C;
extern f32 D_800C0490;

#define MODULE ((LevelScriptState *) ((u8 *) &D_8012F9DC - 0x3D4))

void func_00405C68(CGameObjectInstance *arg0) {
    LevelScriptState *temp_s2;

    if (D_8012F9DC.mode == 2) {
        D_8012F9DC.armed = 1;
        D_8012F9DC.scriptOn = 1;
        MODULE->bossState = 2;
    }
    temp_s2 = MODULE;
    if (MODULE->stage == 1) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C0488;
    }
    if (MODULE->stage == 2) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C048C;
    }
    if (MODULE->stage == 3) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C0490;
    }
    if ((MODULE->stage == 1) && (temp_s2->spawnStep == 0)) {
        func_00236314(&MODULE->anims[3], &D_00431B54);
        func_00236314(&MODULE->anims[4], &D_00431B54);
        func_00236314(&MODULE->anims[6], &D_00431B54);
        func_00236314(&MODULE->anims[5], &D_00431B54);
        temp_s2->spawnStep = (s32) (temp_s2->spawnStep + 1);
    }
    if ((D_8012F608.stage == 2) && (D_8012F608.spawnStep == 1)) {
        func_00236314(&D_8012F608.anims[0], &D_00431B54);
        func_00236314(&D_8012F608.anims[2], &D_00431B54);
        D_8012F608.spawnStep = (s32) (D_8012F608.spawnStep + 1);
    }
    if ((D_8012F608.stage == 3) && (D_8012F608.spawnStep == 2)) {
        func_00236314(&D_8012F608.anims[1], &D_00431B54);
        D_8012F608.spawnStep = (s32) (D_8012F608.spawnStep + 1);
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
