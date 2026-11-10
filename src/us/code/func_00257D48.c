#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "globals/player.h"
#include "protos/scene.h"
#include "protos/ai.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"

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
#else
typedef struct Object Object;
typedef struct AI AI;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void (*update)(Object *obj, AI *ai);
} AIState;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 animType;
} AIAnim;

struct AI {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ AIAnim *anim;
    /* 0x030 */ AIState *state;
    /* 0x034 */ u8 pad34[0x118 - 0x34];
    /* 0x118 */ f32 timer118;
    /* 0x11C */ u8 pad11C[0x128 - 0x11C];
    /* 0x128 */ s32 unk128;
    /* 0x12C */ u8 pad12C[0x130 - 0x12C];
    /* 0x130 */ f32 timer130;
};

struct Object {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *owner;
};


/* First-person weapon AI: counts its timers down, goes to mode 1 when the
 * player's pose asks for it or a weapon change is queued, runs the state's
 * and the weapon's updates, then animates the weapon model. */
void WeaponActor_Update(Object *obj, AI *ai) {
    CPlayer *player;
    s32 mode;

    player = obj->owner;
    mode = gPlayerStateTable[player->m_Mode].pose;
    if (ai->timer130 > 0.0f) {
        ai->timer130 -= gFrameIncrement;
    }
    if (player->m_Data.m_CurrentWeapon != 21) {
        ai->unk128 = 0;
    }
    if (ai->timer118 > 0.0f) {
        ai->timer118 -= gFrameIncrement;
    }
    if (mode == 1) {
        CAI__SetMode(obj, ai, 1);
    }
    if (player->m_RequestedWeapon != player->m_Data.m_CurrentWeapon) {
        player->m_SniperMode = 0;
        CAI__SetMode(obj, ai, 1);
    }
    if (ai->state != NULL && ai->state->update != NULL) {
        ai->state->update(obj, ai);
    }
    if (gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->update != NULL) {
        gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->update(obj, ai);
    }
    CScene__SetInstanceObjectType(&gScene, obj, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->model, ai->anim->animType);
}
#endif
