#include "common.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/model.h"
#include "structs/game_object.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "structs/camera.h"

/* Draws a weapon actor: its muzzle child (given the parent's bounds), then the model. */

extern s32 MenuPool_CountOpen(void *arg0);
extern void EnemyAI_CalcOrientationMatrix(CGameObjectInstance *actor, void *ai);
extern s32 CGameObjectInstance__DrawModel(sptr, sptr, sptr);
extern void CGameObjectInstance__Draw(CGameObjectInstance *actor, struct CCamera *camera);
extern void Render_QueueModelInstance(sptr arg0, sptr arg1, s32 arg2, void *arg3, sptr arg4, s32 arg5, s32 arg6);

void WeaponActor_Draw(CGameObjectInstance *obj, CAI *state, CDrawArgs *args) {
    CPlayer *player = (CPlayer *)obj->m_AI.m_pParent;
    CGameObjectInstance *muzzle;
    s32 arg6 = -1;

    if (gEngineApp_m_Settings.multiplayer != 0 && MenuPool_CountOpen((u8 *)player->m_pCamera->m_MenuPool) != 0) {
        return;
    }
    muzzle = ((CGameObjectInstance *)player->m_pOnEnemy);
    if (muzzle != NULL) {
        EnemyAI_CalcOrientationMatrix(muzzle, (u8 *)muzzle + T2_OFFSETOF(CGameObjectInstance, m_AI));
        muzzle->m_Bounds = obj->m_Bounds;
        muzzle->m_AI.m_pfDraw = (void *)CGameObjectInstance__DrawModel;
        CGameObjectInstance__Draw(muzzle, args->m_pCamera);
        muzzle->m_AI.m_pfDraw = NULL;
    }
    if (player->m_Data.m_CurrentWeapon == 0x15) {
        arg6 = state->m_Weapon.m_nPass;
    }
    Render_QueueModelInstance(args->m_hModel, (sptr)obj->m_pmtDrawMtxs, 1, &obj->m_Lights[gEvenOdd], 0, state->m_Weapon.m_nLod, arg6);
}
