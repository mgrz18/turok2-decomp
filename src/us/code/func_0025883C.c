#include "common.h"
#include "structs/game_object.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/model.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "structs/camera.h"

/* Draws a weapon actor: its muzzle child (copying the parent's render block), then the model. */

typedef struct {
    s32 w[6];
} RenderBlock; /* 0x18 bytes */

typedef struct {
    /* 0x000 */ u8 pad0[0x98];
    /* 0x098 */ s32 model;
    /* 0x09C */ u8 pad9C[0xBC - 0x9C];
    /* 0x0BC */ RenderBlock render;
    /* 0x0D4 */ u8 padD4[0x110 - 0xD4];
    /* 0x110 */ u8 slots[2][0x18];
    /* 0x140 */ u8 pad140[0x1A8 - 0x140];
    /* 0x1A8 */ struct CPlayer *owner;
    /* 0x1AC */ u8 pad1AC[0x234 - 0x1AC];
    /* 0x234 */ void *drawHook;
} WeaponActor;

extern s32 MenuPool_CountOpen(void *arg0);
extern void EnemyAI_CalcOrientationMatrix(WeaponActor *actor, void *ai);
extern s32 CGameObjectInstance__DrawModel(s32, sptr, s32);
extern void CGameObjectInstance__Draw(WeaponActor *actor, struct CCamera *camera);
extern void Render_QueueModelInstance(s32 arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);

void WeaponActor_Draw(WeaponActor *obj, CAI *state, CDrawArgs *args) {
    CPlayer *player = obj->owner;
    WeaponActor *muzzle;
    s32 arg6 = -1;

    if (gEngineApp_m_Settings.multiplayer != 0 && MenuPool_CountOpen(((u8 *)player->m_pCamera) + T2_OFFSETOF(CCamera, m_MenuPool)) != 0) {
        return;
    }
    muzzle = ((WeaponActor *)player->m_pOnEnemy);
    if (muzzle != NULL) {
        EnemyAI_CalcOrientationMatrix(muzzle, (u8 *)muzzle + T2_OFFSETOF(CGameObjectInstance, m_AI));
        muzzle->render = obj->render;
        muzzle->drawHook = CGameObjectInstance__DrawModel;
        CGameObjectInstance__Draw(muzzle, args->m_pCamera);
        muzzle->drawHook = NULL;
    }
    if (player->m_Data.m_CurrentWeapon == 0x15) {
        arg6 = state->m_Weapon.m_nPass;
    }
    Render_QueueModelInstance(args->m_hModel, obj->model, 1, obj->slots[gEvenOdd], 0, state->m_Weapon.m_nLod, arg6);
}
