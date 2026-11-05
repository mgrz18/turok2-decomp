#include "common.h"
#include "structs/player.h"
#include "structs/model.h"

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

extern u8 D_8011AAF1; /* multiplayer */
extern s32 D_800B6D1C;

extern s32 func_00416AF8(void *arg0);
extern void func_0025C018(WeaponActor *actor, void *ai);
extern s32 func_0021D7BC(s32, s32, s32);
extern void func_0021D39C(WeaponActor *actor, struct CCamera *camera);
extern void func_0020B6A0(s32 arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);

void func_0025883C(WeaponActor *obj, CAI *state, CDrawArgs *args) {
    CPlayer *player = obj->owner;
    WeaponActor *muzzle;
    s32 arg6 = -1;

    if (D_8011AAF1 != 0 && func_00416AF8(((u8 *)player->m_pCamera) + 0x53C) != 0) {
        return;
    }
    muzzle = ((WeaponActor *)player->m_pOnEnemy);
    if (muzzle != NULL) {
        func_0025C018(muzzle, (u8 *)muzzle + 0x140);
        muzzle->render = obj->render;
        muzzle->drawHook = func_0021D7BC;
        func_0021D39C(muzzle, args->m_pCamera);
        muzzle->drawHook = NULL;
    }
    if (player->m_Data.m_CurrentWeapon == 0x15) {
        arg6 = state->m_Weapon.m_nPass;
    }
    func_0020B6A0(args->m_hModel, obj->model, 1, obj->slots[D_800B6D1C], 0, state->m_Weapon.m_nLod, arg6);
}
