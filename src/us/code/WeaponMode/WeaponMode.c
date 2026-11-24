/* NOT MATCHING: the ROM's func_00257A3C starts with a stray
 * `addiu $sp, $sp, -0x90` ahead of the real prologue (never undone). The C
 * below reproduces the other 88 words exactly (shifted by that one word). */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/player.h"
#include "structs/game_object.h"
#include "structs/camera.h"

/* The host: the weapon actor itself (scale, orientation, owner in m_AI.m_pParent) and the
 * owner's camera by member; the view below and the camera offsets 0x12C/0x14C are N64 ones. */
typedef CGameObjectInstance WeaponModel;
#define WM_OFFSET(m) (&(m)->m_vScale)
#define WM_MATRIX(m) ((m)->m_mfOrient)
#define WM_OWNER(m) ((CPlayer *)(m)->m_AI.m_pParent)
#define CAMERA_VIEW_ORIENT(cam) ((cam)->m_mfViewOrient)
#define CAMERA_VIEW_ALIGN(cam) (&(cam)->m_qViewAlign)

extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CMtxF__SetScale(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__GetTranslation(void *src, CVector3 *out);
extern void CQuaternion__ToMatrix(void *q, f32 *mtx);
extern void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__SwapYZRows(f32 *mtx);
extern void CMtxF__MultiplyAffine(f32 *dst, f32 *a, f32 *b);

/* Builds the first-person weapon model's matrix from the owner's camera:
 * its own offset scaled by 0.1, the weapon's view offset (optionally
 * mirrored per axis) scaled by -10.24, the camera rotation and position. */
void WeaponActor_BuildMatrix(WeaponModel *model) {
    CVector3 cameraPos;
    f32 rotation[4][4];
    CVector3 scaled;
    CVector3 viewOffset;
    f32 mtx[4][4];
    CPlayer *player;

    player = WM_OWNER(model);
    viewOffset = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->viewOffset;
    if (sWeaponFlipX) {
        viewOffset.x = -viewOffset.x;
    }
    if (sWeaponFlipY) {
        viewOffset.y = -viewOffset.y;
    }
    if (sWeaponFlipZ) {
        viewOffset.z = -viewOffset.z;
    }
    CVector3__Scale(&scaled, WM_OFFSET(model), 0.1f);
    CMtxF__SetScale(mtx[0], scaled.x, scaled.y, scaled.z);
    CVector3__Scale(&viewOffset, &viewOffset, -10.24f);
    CMtxF__GetTranslation(CAMERA_VIEW_ORIENT(player->m_pCamera), &cameraPos);
    CQuaternion__ToMatrix(CAMERA_VIEW_ALIGN(player->m_pCamera), rotation[0]);
    CMtxF__Translate(mtx[0], viewOffset.x, viewOffset.y, viewOffset.z);
    CMtxF__SwapYZRows(mtx[0]);
    CMtxF__MultiplyAffine(WM_MATRIX(model)[0], mtx[0], rotation[0]);
    CMtxF__Translate(WM_MATRIX(model)[0], cameraPos.x, cameraPos.y, cameraPos.z);
}

#include "globals/game.h"
#include "globals/scene.h"
#include "protos/weapon_mode.h"

/* The next weapon the player can switch to: the first usable one after the
 * current, else the last usable one before it (weapons 12 and 23 are never
 * picked going up, 12 never going down); 0x18 or 0 when there is none. */
s32 CPlayer__GetNextUsableWeapon(CPlayer *player) {
    s32 i;
    s32 usable;

    if (player->m_Data.m_CurrentWeapon < gScene_m_PreviewTimer) {
        for (i = player->m_Data.m_CurrentWeapon + 1; i < gScene_m_PreviewTimer; i++) {
            if (CPlayer__GetWeaponAmmo(player, i) && player->m_Data.m_WeaponList[i] && i != 23 && i != 12) {
                if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
                    usable = gWeaponInfoTable[i]->flags & 2;
                } else {
                    usable = gWeaponInfoTable[i]->flags & 1;
                }
                if (usable) {
                    return i;
                }
            }
        }
    }
    for (i = player->m_Data.m_CurrentWeapon - 1; gScene_m_PreviewState < i; i--) {
        if (CPlayer__GetWeaponAmmo(player, i) && player->m_Data.m_WeaponList[i] && i != 12) {
            if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
                usable = gWeaponInfoTable[i]->flags & 2;
            } else {
                usable = gWeaponInfoTable[i]->flags & 1;
            }
            if (usable) {
                return i;
            }
        }
    }
    if (gEngineApp_m_Settings.multiplayer) {
        return 0x18;
    }
    return 0;
}

#include "globals/engine.h"
#include "protos/scene.h"
#include "protos/ai.h"

/* Host body: the N64 body reads the CAI through an AI view at N64 offsets (0x2C/0x30 the mode
 * table/current mode, 0x118/0x128/0x130 the weapon vars) and the owner at instance 0x1A8 (=
 * m_AI.m_pParent). On a 64-bit host its `unk128 = 0` landed in the high half of m_pfStep
 * (CAI 0xF0 + 8 on the host), truncating the step pointer: harmless while the text sits below
 * 4 GB (Linux), a jump to 0x350598 on macOS (round 7, v4). */
void WeaponActor_Update(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player;
    s32 mode;

    player = (CPlayer *) obj->m_AI.m_pParent;
    mode = gPlayerStateTable[player->m_Mode].pose;
    if (ai->m_Weapon.m_RefireDelay > 0.0f) {
        ai->m_Weapon.m_RefireDelay -= gFrameIncrement;
    }
    if (player->m_Data.m_CurrentWeapon != 21) {
        ai->m_Weapon.m_nLod = 0;
    }
    if (ai->m_Weapon.m_FireDelay > 0.0f) {
        ai->m_Weapon.m_FireDelay -= gFrameIncrement;
    }
    if (mode == 1) {
        CAI__SetMode(obj, ai, 1);
    }
    if (player->m_RequestedWeapon != player->m_Data.m_CurrentWeapon) {
        player->m_SniperMode = 0;
        CAI__SetMode(obj, ai, 1);
    }
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ((void (*)(CGameObjectInstance *, CAI *)) ai->m_pModeCurrent->m_pfCallback)(obj, ai);
    }
    if (gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->update != NULL) {
        ((void (*)(CGameObjectInstance *, CAI *)) gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->update)(obj, ai);
    }
    CScene__SetInstanceObjectType(&gScene, obj, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->model,
                                  ai->m_pModeTable->unk_10);
}

extern s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon);

/* Fire check for the first-person weapon: once its cooldown is over and the
 * weapon is usable, fire button 0x2000 starts mode 4 and a tap of 0x10 with
 * weapon 19 starts mode 5. Returns 1 when it fired. */
s32 WeaponActor_CheckFire(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player;
    s32 tapped;
    s32 mode;

    player = (CPlayer *) obj->m_AI.m_pParent;
    if (ai->m_Weapon.m_FireDelay > 0.0f) {
        return 0;
    }
    if (!CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        return 0;
    }
    if (player->m_pOptions->m_TagMode == 1) {
        return 0;
    }
    if ((player->m_Data.m_CurrentWeapon == 13 || player->m_Data.m_CurrentWeapon == 29) && player->m_CrossHairMode != 2) {
        return 0;
    }
    if (player->m_Data.m_CurrentWeapon == 23 && ai->m_Weapon.m_RefireDelay > 0.0f) {
        return 0;
    }
    if (player->m_bHeadMovementDisabled != 0) {
        return 0;
    }
    if (player->m_Data.m_CurrentWeapon == 21) {
        CPlayer *owner = (CPlayer *) obj->m_AI.m_pParent;
        if (*(owner->m_Data.m_AmmoList + *CPlayer__GetFiringAmmo(owner, 21)) == 0) {
            return 0;
        }
    }
    tapped = 0;
    if (player->m_ContState.m_dwButtonsSingle & 0x10) {
        tapped = player->m_Data.m_CurrentWeapon == 19;
    }
    if (player->m_ContState.m_dwButtons & 0x2000) {
        mode = 4;
    } else if (tapped) {
        mode = 5;
    } else {
        return 0;
    }
    CAI__SetMode(obj, ai, mode);
    ai->m_ModeLastFrame = -1;
    return 1;
}

#include "structs/controller.h"
#include "globals/camera.h"

/* Switches the weapon model to the player's next weapon (see WeaponActor_EquipRequested)
 * and puts the weapon AI back in mode 0. */
static __inline__ void switchWeapon(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        ai->m_pModeTable = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->modeTable;
        ai->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        ai->m_Weapon.m_BarrelAngle = 0;
        ai->m_Weapon.m_BarrelSpeed = 0;
        ai->m_Weapon.m_nLod = 0;
        ai->m_Weapon.m_FireDelay = 0;
    }
    CAI__SetMode(obj, ai, 0);
}

/* First-person weapon, idle: picks another weapon when the current one runs
 * out, plays sound 0x27D8 on fire in tag mode, otherwise switches to the
 * queued weapon unless the pose (modes 1 and 7) or visibility forbids it. */
void WeaponMode_Idle_Update(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player;
    s32 mode;

    player = (CPlayer *) obj->m_AI.m_pParent;
    mode = gPlayerStateTable[player->m_Mode].pose;
    if (ai->m_AnimRepeatedCount && !CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        player->m_RequestedWeapon = CPlayer__GetNextUsableWeapon(player);
    }
    player->m_ContState.m_pContState->rumbleBase = 0;
    ai->m_Weapon.m_SpinTime = 0;
    if (gEngineApp_m_Settings_match_tag[0] && player->m_pOptions->m_TagMode) {
        if (player->m_ContState.m_dwButtonsSingle & 0x2000) {
            Sound__Play(0x27D8, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
        }
        return;
    }
    if (mode == 1 || mode == 7) {
        return;
    }
    if (!ai->m_AnimRepeatedCount) {
        return;
    }
    switchWeapon(obj, ai);
}

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Weapon state: sets the fire-rate scale gWeaponChargeScale from the time held for
 * weapons 2-3 (and their 0x1294 ramp) and 12, then goes to state 7 or 5. */
void WeaponMode_WindUp_Update(CGameObjectInstance *obj, CAI *state) {
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;
    f32 t;
    f32 scale;

    if (player->m_ContState.m_dwButtons & 0x2000) {
        return;
    }
    switch (player->m_Data.m_CurrentWeapon) {
    case 2:
    case 3:
        if (player->m_SniperMode) {
            gWeaponChargeScale = 1.5f;
        } else {
            scale = MAX(MIN(state->m_ModeTime * (2.0f / 21), 1.5f), 0.5f);
            gWeaponChargeScale = scale;
            t = state->m_ModeTime * (2.0f / 15) + 0.5f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            player->weapon_particle_damage_scaler = t;
        }
        break;
    case 12:
        gWeaponChargeScale = MAX(MIN(state->m_ModeTime * (2.0f / 21), 2.0f), 0.5f);
        break;
    }
    if (CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) == 1) {
        CAI__SetMode(obj, state, 7);
    } else {
        CAI__SetMode(obj, state, 5);
    }
}

#ifdef TARGET_PORT
#include "structs/camera.h"
#endif

/* Weapon state: winding a spun-up weapon (barrel spin) while fire is held, else stopping it. */



extern void CPlayer__StopWeaponFiringSound(CPlayer *player);
extern void CPlayer__StopWeaponBackgroundSound(CPlayer *player);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);

void WeaponMode_SpinFire_Update(void *obj, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)obj)->m_AI.m_pParent);
#ifdef TARGET_PORT
    CVector3 *pos = &player->m_pCamera->m_vPos; /* N64 camera 0x114 */
#else
    CVector3 *pos = (CVector3 *)(((u8 *)player->m_pCamera) + 0x114);
#endif

    if (!(player->m_ContState.m_dwButtons & 0x2000) || player->m_bHeadMovementDisabled != 0
        || CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) == 0) {
        CAI__SetMode(obj, state, 2);
        if (player->m_pLoopingSound_WeaponFiring != 0) {
            Sound__Play(0x1B5, *pos, pos, -1);
        }
        CPlayer__StopWeaponFiringSound(player);
        CPlayer__StopWeaponBackgroundSound(player);
        Sound__Play(0x1A6, *pos, pos, -1);
        return;
    }
    Math_EaseToward(&state->m_Weapon.m_BarrelSpeed, 0.3490659f, 0.2f);
    if (state->m_Weapon.m_BarrelSpeed < 0.2617994249f) {
        return;
    }
    if (player->m_pLoopingSound_WeaponFiring == 0) {
        CPlayer__StartWeaponFiringSound(player, 0x1A7);
    }
    state->m_ConditionTimer -= gFrameIncrement;
    if (state->m_ConditionTimer <= 0.0f) {
        state->m_ConditionTimer += 1.875f;
        CPlayer__FireWeapon((*(void **)&((CGameObjectInstance *)obj)->m_AI.m_pParent), 0x1A);
    }
}

#define USE_AMMO()                                              {                                                               ammo = &player->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(player, 0x14)];         n = *ammo - 1;                                              if (n < 0) {                                                    n = 0;                                                  }                                                           *ammo = n;                                              }

void WeaponMode_StreamFire_Update(void *obj, CAI *state) {
    CPlayer *player = (CPlayer *) ((CGameObjectInstance *) obj)->m_AI.m_pParent;
    s16 *ammo;
    s32 n;

    if (!(player->m_ContState.m_dwButtons & 0x2000) || CPlayer__GetWeaponAmmo(player, 0x14) == 0) {
        if (state->m_ModeTime < 3.0f) {
            USE_AMMO();
        }
        CPlayer__StopWeaponFiringSound(player);
        CAI__SetMode(obj, state, 2);
        return;
    }
    if (state->m_ModeTime > 30.0f) {
        player->m_FleeTime = 45.0f;
    }
    if (gEvenOdd != 0) {
        state->m_ConditionTimer += 1.5f;
        CPlayer__FireWeapon(((CGameObjectInstance *) obj)->m_AI.m_pParent, 0x14);
    }
    if (gFrameCountGameplay % 3 == 0) {
        USE_AMMO();
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CAI__SetMode receives WeaponActor_SetModeWrapper's own argument
 * registers (pass-through); the host passes them. */
void WeaponActor_SetModeWrapper(void * pt0, void * pt1, s32 pt2) {
    CAI__SetMode(pt0, pt1, pt2);
}

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

/* Switches the weapon actor to its owner's requested weapon. */
void WeaponActor_SwitchToRequested(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)weapon)->m_AI.m_pParent);
    s16 requested = player->m_RequestedWeapon;

    if (player->m_Data.m_CurrentWeapon != requested || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->m_pModeTable = gWeaponInfoTable[requested]->modeTable;
        state->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
}

/* Weapon callback: forwards to the owning player (m_AI.m_pParent). */
void WeaponActor_FireCallback(CGameObjectInstance *arg0, sptr arg1, s32 arg2) {
    CPlayer__FireWeapon((sptr)arg0->m_AI.m_pParent, arg2);
}

/* WeaponActor_SwitchToRequested, inlined: switches the weapon actor to its owner's
 * requested weapon. */
static __inline__ void SwitchWeapon(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)weapon)->m_AI.m_pParent);
    s16 requested = player->m_RequestedWeapon;

    if (player->m_Data.m_CurrentWeapon != requested || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->m_pModeTable = gWeaponInfoTable[requested]->modeTable;
        state->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
}

void WeaponActor_SwitchToRequestedInline(CGameObjectInstance *weapon, CAI *state) {
    SwitchWeapon(weapon, state);
    CAI__SetMode(weapon, state, 0);
}

#include "structs/model.h"

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

#include "m2c_macros.h"

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

/* Same body as WeaponActor_SwitchToRequested. Switches the weapon actor to its owner's requested weapon. */
void WeaponMode_Switch_Enter(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)weapon)->m_AI.m_pParent);
    s16 requested = player->m_RequestedWeapon;

    if (player->m_Data.m_CurrentWeapon != requested || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->m_pModeTable = gWeaponInfoTable[requested]->modeTable;
        state->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
}

/* the CPlayer words by member on the host (N64 0xB2C m_CrossHairMode, 0xB30 m_CrossHairTimer,
 * 0xB38 m_pCrossHairTarget, cleared as a pointer, 0xA64 m_ContState.m_dwButtons) */
#define PL_CROSSHAIR_MODE(p) ((p)->m_CrossHairMode)
#define PL_CROSSHAIR_TARGET(p) (*(sptr *) &(p)->m_pCrossHairTarget)
#define PL_CROSSHAIR_TIMER(p) ((p)->m_CrossHairTimer)
#define PL_BUTTONS(p) ((p)->m_ContState.m_dwButtons)

void WeaponMode_Switch_Update(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)weapon)->m_AI.m_pParent);

    PL_CROSSHAIR_MODE(player) = 1;
    PL_CROSSHAIR_TARGET(player) = 0;
    PL_CROSSHAIR_TIMER(player) = 0;
    state->m_Weapon.m_BarrelAngle = 0;
    state->m_Weapon.m_BarrelSpeed = 0;
    if (state->m_AnimRepeatedCount != 0) {
        if ((gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->flags & 4) && WeaponActor_CheckFire(weapon, state) &&
            CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) && (PL_BUTTONS(player) & 0x2000) &&
            ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
            CAI__SetMode(weapon, state, 4);
        } else {
            CAI__SetMode(weapon, state, 2);
        }
    }
}

void WeaponMode_Ready_Update(CGameObjectInstance *arg0, sptr arg1) {
    s32 temp_s2;
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *)arg0->m_AI.m_pParent;
    temp_s2 = gPlayerStateTable[temp_s0->m_Mode].pose;
    if (CPlayer__GetWeaponAmmo((sptr) temp_s0, (s32) temp_s0->m_Data.m_CurrentWeapon) == 0) {
        temp_s0->m_RequestedWeapon = CPlayer__GetNextUsableWeapon((sptr) temp_s0);
    }
    if ((WeaponActor_CheckFire((sptr) arg0, arg1) == 0) && !(arg0->m_dwFlags & 0x400)) {
        CAI__SetMode(arg0, (CAI *) arg1, (s32) temp_s2);
    }
}

void WeaponMode_Fire_Enter(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_Weapon.m_FireDelay = (f32) (gWeaponInfoTable[((CPlayer *) arg0->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f);
}

void WeaponMode_Fire_Update(CGameObjectInstance *obj, CAI *ai) {
    s16 temp_a1;
    CPlayer *temp_s1;

    temp_s1 = (CPlayer *) obj->m_AI.m_pParent;
    if (ai->m_AnimRepeatedCount != 0) {
        if ((*(f32 *) &ai->m_VarBuffer[3] <= 0.0f) && (temp_a1 = temp_s1->m_Data.m_CurrentWeapon, ((gWeaponInfoTable[temp_a1]->flags & 4) != 0)) && (CPlayer__GetWeaponAmmo((sptr) temp_s1, (s32) temp_a1) != 0) && (WeaponActor_CheckFire((sptr) obj, (sptr) ai) != 0)) {
            if ((temp_s1->m_ContState.m_dwButtons & 0x2000) && (temp_s1->m_pOptions->m_TagMode == 0)) {
                ai->m_AnimRepeatedCount = 0;
                ai->m_ModeLastFrame = -1;
                ai->m_Mode = -1;
                CAI__SetMode(obj, ai, 4);
                *(f32 *) &ai->m_VarBuffer[3] = (f32) (gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f);
            } else {
                goto block_9;
            }
        } else {
block_9:
            CAI__SetMode(obj, ai, 2);
        }
        temp_s1->m_CrossHairMode = 1;
        temp_s1->m_CrossHairTimer = 0;
    }
}

s32 CPlayer__StopLoopingSounds(sptr);                         /* extern */

void WeaponMode_Idle_Enter(CGameObjectInstance *arg0) {
    CPlayer__StopLoopingSounds((sptr) arg0->m_AI.m_pParent);
}

s32 WeaponAnim_4EF2(void) {
    return 0x4EF2;
}

s32 WeaponAnim_4E84(void) {
    return 0x4E84;
}

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */

extern void CMtxF__RotateY(void *arg0, f32 arg1);

void WeaponBarrel_NodeMatrix_AEE38(void *arg0, T2PoseContext *state) {
    f32 value = state->obj->m_AI.m_Weapon.m_BarrelAngle;
    f32 half = value * 0.5f;
    f32 more = value * 1.5f;

    switch (state->bone) {
    case 1:
        CMtxF__PreRotateY(arg0, half);
        break;
    case 7:
        CMtxF__RotateY(arg0, more);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        CMtxF__PreRotateY(arg0, -more);
        break;
    }
}
#else
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 mode;
} WeaponState;

extern void CMtxF__RotateY(void *arg0, f32 arg1);

void WeaponBarrel_NodeMatrix_AEE38(void *arg0, WeaponState *state) {
    f32 value = state->owner->m_AI.m_Weapon.m_BarrelAngle;
    f32 half = value * 0.5f;
    f32 more = value * 1.5f;

    switch (state->mode) {
    case 1:
        CMtxF__PreRotateY(arg0, half);
        break;
    case 7:
        CMtxF__RotateY(arg0, more);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        CMtxF__PreRotateY(arg0, -more);
        break;
    }
}
#endif

void WeaponMode_SpinUp_Enter(CGameObjectInstance *obj, CAI *ai) {
    sptr player;

    player = (sptr) obj->m_AI.m_pParent;
    ai->m_ConditionTimer = 0.0f;
    CPlayer__StartWeaponBackgroundSound(player, 0x1A5);
    *(f32 *) &ai->m_VarBuffer[3] = gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
}

extern f32 Math_DecayToZero(f32 value, f32 rate);  /* moves value toward zero */
#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* Spins down unless held (state 4), turns the angle and drives the rumble
 * (the controller's rumbleBase) and the spin sound from the spin speed. */
void WeaponBarrel_Spin_AEE38(CGameObjectInstance *actor, CAI *sp) {
    CPlayer *owner;
    ControllerProfile *pad;

    owner = ((CPlayer *)actor->m_AI.m_pParent);
    if (sp->m_Mode != 4) {
        sp->m_Weapon.m_BarrelSpeed = Math_DecayToZero(sp->m_Weapon.m_BarrelSpeed, 0.0065449853f);
    }
    sp->m_Weapon.m_BarrelAngle += sp->m_Weapon.m_BarrelSpeed * gFrameIncrement * 2.0f;
    pad = owner->m_ContState.m_pContState;
    pad->rumbleBase = (ABS(sp->m_Weapon.m_BarrelSpeed) * 1.7904929f > 1.0f) ? 1.0f : ABS(sp->m_Weapon.m_BarrelSpeed) * 1.7904929f;
    if (sp->m_Mode != 4 && (gFrameCountGameplay & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */


void WeaponNode_Rotate_AF324(void *mtx, T2PoseContext *arg1) {
    f32 angle = arg1->obj->m_AI.m_Weapon.m_BarrelAngle * 1.5f; /* N64 instance 0x24C */

    if (arg1->bone == 2) {
        CMtxF__PreRotateY(mtx, -angle);
    }
}
#else

void WeaponNode_Rotate_AF324(void *mtx, void *arg1) {
    f32 angle = *(f32 *) (*(s8 **) arg1 + 0x24C) * 1.5f;

    if (*(s32 *) ((s8 *) arg1 + 0x90) == 2) {
        CMtxF__PreRotateY(mtx, -angle);
    }
}
#endif

void WeaponMode_AE634_Enter(CGameObjectInstance *obj, CAI *ai) {
    sptr temp_s1;

    temp_s1 = (sptr) obj->m_AI.m_pParent;
    ai->m_ConditionTimer = 0.0f;
    CPlayer__FireWeapon((sptr) obj->m_AI.m_pParent, 0x12B);
    CPlayer__StartWeaponBackgroundSound(temp_s1, 0x1A5);
    CPlayer__StartWeaponFiringSound(temp_s1, 0x1A7);
    *(f32 *) &ai->m_VarBuffer[3] = gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
}

void WeaponMode_SpinUpFire_Update(void *obj, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)obj)->m_AI.m_pParent);
#ifdef TARGET_PORT
    CVector3 *pos = &player->m_pCamera->m_vPos; /* N64 camera 0x114 */
#else
    CVector3 *pos = (CVector3 *)(((u8 *)player->m_pCamera) + 0x114);
#endif

    if (!(player->m_ContState.m_dwButtons & 0x2000) || player->m_bHeadMovementDisabled != 0
        || CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) == 0) {
        CAI__SetMode(obj, state, 2);
        CPlayer__StopWeaponBackgroundSound(player);
        CPlayer__StopWeaponFiringSound(player);
        Sound__Play(0x1A6, *pos, pos, -1);
        return;
    }
    if (player->m_pLoopingSound_WeaponFiring == 0) {
        CPlayer__StartWeaponFiringSound(player, 0x1A7);
    }
    Math_EaseToward(&state->m_Weapon.m_BarrelSpeed, 0.2792527f, 0.4f);
    state->m_ConditionTimer -= gFrameIncrement;
    if (state->m_ConditionTimer <= 0.0f) {
        state->m_ConditionTimer += 1.875f;
        CPlayer__FireWeapon((*(void **)&((CGameObjectInstance *)obj)->m_AI.m_pParent), 0x12B);
    }
}

f32 Math_DecayToZero(f32 value, f32 rate);

/* A weapon's spinning barrel (an AI callback): the weapon actor's CAI keeps the mode and
 * the barrel's angle and speed (m_Weapon), the actor's parent is the player, whose
 * controller state takes the rumble. */
void WeaponBarrel_Spin_AF324(CGameObjectInstance *obj, CAI *s) {
    CPlayer *owner = (CPlayer *) obj->m_AI.m_pParent;
    struct ControllerProfile *target;

    if (s->m_Mode != 4) {
        s->m_Weapon.m_BarrelSpeed = Math_DecayToZero(s->m_Weapon.m_BarrelSpeed, 0.0065449853f);
    }
    s->m_Weapon.m_BarrelAngle += s->m_Weapon.m_BarrelSpeed * gFrameIncrement * 2;
    target = owner->m_ContState.m_pContState;
    target->rumbleBase = (ABS(s->m_Weapon.m_BarrelSpeed) * 1.7904929f > 1.0f) ? 1.0f : ABS(s->m_Weapon.m_BarrelSpeed) * 1.7904929f;
    if (s->m_Mode != 4 && (gFrameCountGameplay & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}

sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);        /* extern */
s32 CGameObjectInstance__SetAnim(void *, sptr, s32);        /* extern */

void WeaponActor_PlayChargeAnim(CGameObjectInstance *obj) {
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *) obj->m_AI.m_pParent;
    if (temp_s0->m_ContState.m_dwButtons & 0x2000) {
        temp_s0->m_CharAnim = 0x1130;
        CGameObjectInstance__SetAnim(temp_s0, CGameObjectInstance__PickRandomAnim(temp_s0, 0x1130, -1), 0);
        temp_s0->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
    }
}

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */


void WeaponNode_Rotate_AF084(sptr arg0, T2PoseContext *arg1) {
    if (arg1->bone == 2) {
        CMtxF__PreRotateY((void *) arg0, arg1->obj->m_AI.m_Weapon.m_BarrelAngle);
    }
}
#else
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 state;
} Arg1;


void WeaponNode_Rotate_AF084(sptr arg0, Arg1 *arg1) {
    if (arg1->state == 2) {
        CMtxF__PreRotateY(arg0, arg1->owner->m_AI.m_Weapon.m_BarrelAngle);
    }
}
#endif

extern f32 Math_Approach(f32 value, f32 target, f32 rate);

void WeaponActor_Sway_AF084(void *obj, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)obj)->m_AI.m_pParent);
    f32 target;
    f32 rate;
    f32 dir;
    f32 tilt;

    if (player->m_ContState.m_FinalMoveY > 0.0f && state->m_Mode != 1) {
        target = -0.41887906f;
        if (player->m_Speed > 0.0f) {
            target = 0.41887906f;
        }
        dir = 1.0f;
        rate = player->m_TotalSpeed * 0.0525f;
        if (player->m_Speed < 0.0f) {
            dir = -1.0f;
        }
        if (state->m_Weapon.m_BarrelSpeed < 0.0f ? dir != -1.0f : dir != 1.0f) {
            rate *= 4.0f;
        }
        if (player->m_pLoopingSound_WeaponBackground == 0) {
            CPlayer__StartWeaponBackgroundSound(player, 0x467);
        }
    } else {
        rate = 0.008726647124f;
        target = -0.017453294f;
        if (state->m_Weapon.m_BarrelSpeed > 0.0f) {
            target = 0.017453294f;
        }
        if (player->m_pLoopingSound_WeaponBackground != 0) {
            CPlayer__StopWeaponBackgroundSound(player);
        }
    }
    tilt = Math_Approach(state->m_Weapon.m_BarrelSpeed, target, rate);
    state->m_Weapon.m_BarrelSpeed = tilt;
    state->m_Weapon.m_BarrelAngle -= tilt;
}

s32 WeaponAnim_Alternate4F1A(sptr arg0, CAI *ai) {
    f32 temp_f0;
    s32 var_v0;

    var_v0 = 0x4F1A;
    temp_f0 = -*(f32 *) &ai->m_VarBuffer[2];
    *(f32 *) &ai->m_VarBuffer[2] = temp_f0;
    if (!(temp_f0 > 0.0f)) {
        var_v0 = 0x4F24;
    }
    return var_v0;
}

void WeaponMode_AE694_Update(CGameObjectInstance *arg0, CAI *arg1) {
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *) arg0->m_AI.m_pParent;
    if (CPlayer__GetWeaponAmmo(temp_s0, temp_s0->m_Data.m_CurrentWeapon) == 0) {
        CAI__SetMode(arg0, arg1, 2);
        return;
    }
    if (temp_s0->m_ContState.m_dwButtonsSingle & 0x10) {
        CAI__SetMode(arg0, arg1, 5);
        return;
    }
    if (!(temp_s0->m_ContState.m_dwButtons & 0x2000)) {
        CAI__SetMode(arg0, arg1, 2);
    }
    if (arg1->m_AnimRepeatedCount != 0) {
        arg1->m_ModeLastFrame = -1;
    }
}

void WeaponMode_AE694_Hold(CGameObjectInstance *actor, void *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if ((player->m_ContState.m_dwButtons & 0x2000) && CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        CAI__SetMode(actor, state, 4);
    } else if (!(player->m_ContState.m_dwButtons & 0x10)) {
        CAI__SetMode(actor, state, 2);
    }
}

/* Host body: the N64 State view reads the CAI at N64 offsets (mode table 0x2C, effects 0x108,
 * the weapon vars at 0x10C..); on the host they are members (m_Weapon: CWeaponAIVars). */

void WeaponActor_EquipRequested(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->m_pModeTable = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->modeTable;
        state->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, (sptr) player->m_pCamera,
                                 (sptr) gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
    CPlayer__StartWeaponBackgroundSound(player, 0x1A8);
}

/* Host body: the State view's 0x64 is m_ConditionTimer (cleared as a word), 0x118 m_Weapon.m_FireDelay. */

void WeaponMode_AE714_Enter(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    *(s32 *) &state->m_ConditionTimer = 0;
    state->m_Weapon.m_FireDelay = gWeaponInfoTable[((CPlayer *)actor->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
    CPlayer__StartWeaponFiringSound(player, 0x1A9);
}

void WeaponSound_Update_AF12C(CGameObjectInstance *arg0, CAI *arg1) {
    CPlayer *temp_a0;

    temp_a0 = (CPlayer *) arg0->m_AI.m_pParent;
    if (temp_a0->m_pLoopingSound_WeaponBackground == 0) {
        if (arg1->m_Mode != 1) {
            CPlayer__StartWeaponBackgroundSound(temp_a0, 0x1A8);
        }
    }
    if ((arg1->m_Mode != 4) && (gFrameCountGameplay & 1)) {
        Sound__StopSoundId(0x1A9);
    }
}

void WeaponMode_AE794_Enter(CGameObjectInstance *arg0) {
    CPlayer__StartWeaponBackgroundSound((sptr) arg0->m_AI.m_pParent, 0x464);
}

/* Host body: the State view's 0x110 is m_Weapon.m_BarrelSpeed, 0x118 m_Weapon.m_FireDelay. */
void CPlayer__StopWeaponBackgroundSound(CPlayer *player);

void WeaponMode_AE794_Update(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_pLoopingSound_WeaponBackground == 0) {
        CPlayer__StartWeaponBackgroundSound(player, 0x464);
    }
    if (!(player->m_ContState.m_dwButtons & 0x2000)) {
        CPlayer__StopWeaponBackgroundSound(player);
        if (state->m_Weapon.m_BarrelSpeed < 0.27925270795822144f /* 16 deg */ || state->m_Weapon.m_FireDelay > 0.0f) {
            CAI__SetMode(actor, state, 2);
        } else {
            state->m_Weapon.m_FireDelay = gWeaponInfoTable[22]->fireDelay * 15.0f;
            CAI__SetMode(actor, state, 5);
        }
    }
}

extern f32 func_002982F0(f32 x); /* sin */

void WeaponBarrel_Spin_AF1D4(void *obj, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)obj)->m_AI.m_pParent);
    f32 t;

    state->m_Weapon.m_BarrelAngle += state->m_Weapon.m_BarrelSpeed * gFrameIncrement;
    if (state->m_Mode == 4) {
        Math_EaseToward(&state->m_Weapon.m_BarrelSpeed, 0.47123894f, 0.025f);
    } else {
        state->m_Weapon.m_BarrelSpeed = Math_Approach(state->m_Weapon.m_BarrelSpeed, 0.08726647496f, 0.008f);
    }
    state->m_Weapon.m_SpinTime += gFrameIncrement;
    t = state->m_Weapon.m_BarrelSpeed * 2.122065783f;
    player->m_ContState.m_pContState->rumbleBase = t * t * (func_002982F0(state->m_Weapon.m_BarrelAngle * 1.5f) * 0.4f + 0.4f) + t * 0.05f;
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */


void WeaponNode_Rotate_AF1D4(void *mtx, T2PoseContext *bone) {
    f32 angle = bone->obj->m_AI.m_Weapon.m_BarrelAngle; /* N64 instance 0x24C */

    switch (bone->bone) {
    case 1:
        CMtxF__PreRotateY(mtx, -angle);
        break;
    case 12:
        CMtxF__PreRotateY(mtx, angle + angle);
        break;
    }
}
#else
#include "structs/weapon_node.h"


void WeaponNode_Rotate_AF1D4(void *mtx, WeaponNodeBone *bone) {
    f32 angle = bone->anim->angle;

    switch (bone->type) {
    case 1:
        CMtxF__PreRotateY(mtx, -angle);
        break;
    case 12:
        CMtxF__PreRotateY(mtx, angle + angle);
        break;
    }
}
#endif
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */

void CMtxF__PreRotateZ(void *mtx, f32 angle);

void WeaponNode_Rotate_AF180(void *mtx, T2PoseContext *bone) {
    if (bone->bone == 2) {
        CMtxF__PreRotateZ(mtx, -bone->obj->m_AI.m_Weapon.m_BarrelAngle * 0.5f);
    }
}
#else
#include "structs/weapon_node.h"

void CMtxF__PreRotateZ(void *mtx, f32 angle);

void WeaponNode_Rotate_AF180(void *mtx, WeaponNodeBone *bone) {
    if (bone->type == 2) {
        CMtxF__PreRotateZ(mtx, -bone->anim->angle * 0.5f);
    }
}
#endif
#ifdef TARGET_PORT
#include "globals/engine.h"
/* Host body: the State view is the CAI (m_Mode 0x34, m_AnimType 0xC4, the weapon vars
 * 0x10C/0x110/0x128/0x12C); the player's ammo by member (N64 0x934 = m_Data.m_AmmoList). */
f32 Math_Approach(f32 value, f32 target, f32 rate);
s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 id);

void WeaponBarrel_Spin_AF180(CGameObjectInstance *actor, CAI *state) {
    f32 v;

    v = Math_Approach(state->m_Weapon.m_BarrelSpeed, 0.174532949924469f /* 10 deg */, 0.0175f);
    state->m_Weapon.m_BarrelSpeed = v;
    state->m_Weapon.m_BarrelAngle += v * gFrameIncrement;
    if (state->m_Mode == 4 && state->m_AnimType != 0x4E84) {
        if (7.0f <= actor->m_CurrentAnimation.m_AnimationFrame) {
            if (state->m_Weapon.m_nLod == 0) {
                state->m_Weapon.m_nPass = 0;
            }
            state->m_Weapon.m_nLod = 1;
        } else {
            state->m_Weapon.m_nLod = 0;
        }
    } else {
        CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

        if (player->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(player, 0x15)] != 0) {
            state->m_Weapon.m_nLod = 0;
        } else {
            state->m_Weapon.m_nLod = 1;
        }
    }
}
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 unk34;
    /* 0x035 */ u8 pad35[0xC4 - 0x35];
    /* 0x0C4 */ s16 unkC4;
    /* 0x0C6 */ u8 padC6[0x10C - 0xC6];
    /* 0x10C */ f32 unk10C;
    /* 0x110 */ f32 unk110;
    /* 0x114 */ u8 pad114[0x128 - 0x114];
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
} State;

#include "globals/engine.h"
/* Frame globals; the time step (gFrameIncrement) must be read through the block to match. */
typedef struct {
    /* 0x0 */ u32 frame;
    /* 0x4 */ u32 pad4;
    /* 0x8 */ f32 delta;
} FrameBlock;
#define sFrameBlock (*(FrameBlock *) &gFrameCountGameplay)
f32 Math_Approach(f32 value, f32 target, f32 rate);
s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 id);

void WeaponBarrel_Spin_AF180(CGameObjectInstance *actor, State *state) {
    f32 v;

    v = Math_Approach(state->unk110, 0.174532949924469f /* 10 deg */, 0.0175f);
    state->unk110 = v;
    state->unk10C += v * sFrameBlock.delta;
    if (state->unk34 == 4 && state->unkC4 != 0x4E84) {
        if (7.0f <= actor->m_CurrentAnimation.m_AnimationFrame) {
            if (state->unk128 == 0) {
                state->unk12C = 0;
            }
            state->unk128 = 1;
        } else {
            state->unk128 = 0;
        }
    } else {
        CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

        if (*(s16 *) ((s8 *) player + *CPlayer__GetFiringAmmo(player, 0x15) * 2 + 0x934) != 0) {
            state->unk128 = 0;
        } else {
            state->unk128 = 1;
        }
    }
}
#endif

void WeaponMode_AE7F4_Enter(CGameObjectInstance *obj, CAI *ai) {
    CVector3 *temp_v0;
    CCamera *temp_v1;

    temp_v1 = ((CPlayer *) obj->m_AI.m_pParent)->m_pCamera;
    temp_v0 = &temp_v1->m_vPos;
    Sound__Play(0x1BB, *temp_v0, (sptr) temp_v0, -1);
    ai->m_ModeTime = 0.0f;
    *(f32 *) &ai->m_VarBuffer[3] = (f32) (gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f);
}

extern s16 sChargeShotRecord_count;
extern f32 Random_Blend(f32 lo, f32 hi); /* random in range */

/* Weapon state: on release, charges by the time held (0.1 + t/45, at most 1)
 * and sets the shot count sChargeShotRecord_count (charge * 3, at least 1), then goes to
 * state 5. */
void WeaponMode_Charge_Update(CGameObjectInstance *obj, CAI *state) {
    f32 base = 0.1f;
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;
    f32 charge;
    f32 n;
    f32 t;
    s16 shots;

    player->m_ContState.m_pContState->rumbleBase = Random_Blend(base, 0.3f);
    if (player->m_ContState.m_dwButtons & 0x2000) {
        return;
    }
    t = state->m_ModeTime * (1.0f / 45) + base;
    charge = MIN(t, 1.0f);
    player->weapon_particle_damage_scaler = charge;
    n = charge * 3.0f;
    shots = (n < 1.0f) ? 1 : (s32) n;
    sChargeShotRecord_count = shots;
    CAI__SetMode(obj, state, 5);
    player->m_ContState.m_pContState->rumbleBase = 0.0f;
}

s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon);

void WeaponMode_Weapon10Fire_Enter(CGameObjectInstance *actor, void *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);
    s16 *ammo = &player->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(player, 0x10)];

    if (*ammo > 0) {
        (*ammo)--;
    }
    if (CPlayer__GetWeaponAmmo(player, 0x10) == 0) {
        CAI__SetMode(actor, state, 7);
    } else {
        CAI__SetMode(actor, state, 4);
    }
}

void WeaponMode_Weapon1FFire_Enter(CGameObjectInstance *actor, void *state) {
    if (CPlayer__GetWeaponAmmo(actor->m_AI.m_pParent, 0x1F) == 1) {
        CAI__SetMode(actor, state, 7);
    } else {
        CAI__SetMode(actor, state, 4);
    }
}

void WeaponMode_AE934_Enter(sptr arg0, CAI *ai) {
    ai->m_Weapon.m_RefireDelay = 9.0e1f;
    CAI__SetMode(arg0, ai, 4);
}
