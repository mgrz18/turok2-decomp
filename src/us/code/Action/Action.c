#include "common.h"
#include "structs/player.h"
#include "globals/text.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ struct CIntelligence *def;
} Pickup;
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s8 taken;
} PickupState;
typedef struct {
    /* 0x000 */ u8 kind; /* 1: player */
    /* 0x001 */ u8 pad1[0xD3];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0xD0];
    /* 0x1A8 */ CPlayer *owner;
} Toucher;
extern u8 gEngineApp_m_Game_m_CameraPool[]; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */ /* game */
extern u8 gEngineApp_m_Game_m_CameraPool_m_MainCamera[]; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */ /* game + 0x40: fallback camera */
extern s32 CPlayer__GetPickupCountLow(CPlayer *player, s32 item); /* can take it? */
extern void CPlayer__UseSpecialPickup(CPlayer *player, s32 item);
extern void CCameraPool__AddMultiLineText(void *game, void *camera, char *text, Pickup *obj, f32 time);
extern void Sound__PlaySpeech(s32 sound);
/* A touch on a pickup: give the item, or show why it cannot be taken. */
#include "structs/game_object.h"
#include "structs/camera.h"

/* The port: the pickup, its AI and the toucher by member of CGameObjectInstance/CAI (N64: the
 * definition 0x14, the AI's m_Mode 0x34, the toucher's type 0, flags 0xD4, its AI's m_pParent
 * 0x1A8 = the player); the definition's action body (CActionIntelligence: the item it needs, the message without it); the
 * fallback camera by member of the camera pool (the N64 symbol is the pool + 0x40). */
s32 Action_CheckRequiredItem(Pickup *obj_, PickupState *state_, Toucher *toucher_) {
    CGameObjectInstance *obj = (CGameObjectInstance *) obj_;
    CAI *state = (CAI *) state_;
    CGameObjectInstance *toucher = (CGameObjectInstance *) toucher_;
    CActionIntelligence *info = &obj->header.m_pIntelligence->m_Action;
    CPlayer *player;
    void *camera;

    if (info->m_GoPickupNeeded == 0 || state->m_Mode != 0 || toucher->header.m_Type != 1 || !(toucher->m_dwFlags & 0x300000)) {
        return 1;
    }
    player = (CPlayer *) toucher->m_AI.m_pParent;
    if (CPlayer__GetPickupCountLow(player, info->m_GoPickupNeeded)) {
        CPlayer__UseSpecialPickup(player, info->m_GoPickupNeeded);
        return 1;
    }
    if (info->m_GoPickupNeededText == 0) {
        return 0;
    }
    camera = &((CCameraPool *) gEngineApp_m_Game_m_CameraPool)->m_MainCamera;
    if (player->m_pCamera != NULL) {
        camera = player->m_pCamera;
    }
    CCameraPool__AddMultiLineText(gEngineApp_m_Game_m_CameraPool, camera,
                  gLevelMessages[info->m_GoPickupNeededText].text, obj_, 4.0f);
    if (gLevelMessages[info->m_GoPickupNeededText].speech != 0) {
        Sound__PlaySpeech(gLevelMessages[info->m_GoPickupNeededText].speech);
    }
    return 0;
}

/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Action_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}

#include "globals/ai.h"
#include "structs/model.h"

void Action_Draw(u8 *actor, u8 *state, CDrawArgs *req);

/* AI init: mode table, step/draw callbacks, cleared mode scratch. */
void Action_Setup(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_pModeTable = gActionModeTable;
    arg1->m_pfStep = Action_Step;
    arg1->m_pfDraw = Action_Draw;
    arg1->m_VarBuffer[0] = 0;
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    if (arg0->header.m_pIntelligence->m_dwTypeFlags & 1) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
        return;
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}

#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the type id (N64 0xB8) and the flags (0xD4) by member of CGameObjectInstance. */
void Action_Advance(u8 *actor_, u8 *state) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;

    if (CScene__GetInstanceSavedBit(&gScene, actor) == 1) {
        CAI__SetMode(actor, state, 1);
        switch ((u16) actor->m_nTypeFlag) {
            case 0x64C:
                actor->m_dwFlags &= ~0x2000;
                actor->m_dwFlags &= ~0x100;
                break;
            case 0x64B:
            case 0x64E:
                actor->m_dwFlags &= ~0x8000000;
                break;
        }
    } else {
        CAI__SetMode(actor, state, 0);
    }
}
#else
void Action_Advance(u8 *actor, u8 *state) {
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 1) {
        CAI__SetMode(actor, state, 1);
        switch (*(u16 *) (actor + 0xB8)) {
            case 0x64C:
                *(s32 *) (actor + 0xD4) &= ~0x2000;
                *(s32 *) (actor + 0xD4) &= ~0x100;
                break;
            case 0x64B:
            case 0x64E:
                *(s32 *) (actor + 0xD4) &= ~0x8000000;
                break;
        }
    } else {
        CAI__SetMode(actor, state, 0);
    }
}
#endif
#include "structs/intelligence.h"

s32 Action_GetIdleAnim(CGameObjectInstance *actor) {
    s32 v = actor->header.m_pIntelligence->m_Action.m_IdleAnim;
    if (v != 0) {
        actor->m_dwFlags |= 0x2000;
    } else {
        actor->m_dwFlags &= ~0x2000;
    }
    return v;
}

#include "protos/game.h"

void Action_IdleEnter(sptr arg0, CAI *arg1) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
    GameObject_FireTriggers(arg0, 0x200000, arg0);
    arg1->m_VarBuffer[2] = 0;
}

/* Turok 2: def at 0x14, flags at 0xD4 (Rage Wars 0x18 / 0x100). */
s32 Action_GetGoAnim(CGameObjectInstance *actor) {
    s32 v = actor->header.m_pIntelligence->m_Action.m_GoAnim;
    if (v != 0) {
        actor->m_dwFlags |= 0x2000;
    } else {
        actor->m_dwFlags &= ~0x2000;
        actor->m_dwFlags &= ~0x100;
    }
    return v;
}

#include "globals/swoosh.h"

void SwooshSet_FreeOwner(void *list, void *actor);
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* The port: the intelligence (N64 0x14) and the flags (0xD4) by member of CGameObjectInstance;
 * the definition's flags (0x14) and go animation (0x28) as CActionIntelligence. Reached when a
 * level loads with an action already used (its saved bit set): the warp back into level 51
 * crashed here once the level triggers worked (round 7 merge). */
void Action_GoEnter(u8 *actor_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CIntelligence *def = actor->header.m_pIntelligence;

    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 0x80000, (sptr) actor);
    SwooshSet_FreeOwner(&gEngineApp_m_SwooshSystem, actor);
    if (gScene.m_Mode != 4) {
        actor->m_dwFlags |= 0x8000000;
        if (def->m_Action.m_dwFlags & 2) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (def->m_Action.m_GoAnim == 0) {
            actor->m_dwFlags &= ~0x100;
        }
    }
}
#else
void Action_GoEnter(u8 *actor) {
    s32 *table = &gScene;
    u8 *def = *(u8 **) (actor + 0x14);

    CScene__SetInstanceSavedBit(table, actor, 1);
    GameObject_FireTriggers(actor, 0x80000, actor);
    SwooshSet_FreeOwner(&gEngineApp_m_SwooshSystem, actor);
    if (*table != 4) {
        *(s32 *) (actor + 0xD4) |= 0x8000000;
        if (*(s32 *) (def + 0x14) & 2) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s16 *) (*(u8 **) (actor + 0x14) + 0x28) == 0) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
    }
}
#endif
#include "structs/game.h"
#include "globals/game.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 unk34;
    /* 0x035 */ u8 pad35;
    /* 0x036 */ s8 unk36;
    /* 0x037 */ u8 pad37[0x90];
    /* 0x0C7 */ s8 active;
    /* 0x0C8 */ u8 padC8[0x48];
    /* 0x110 */ s32 done;
    /* 0x114 */ s32 tagCleared;
} ActionState;
extern CPlayer *CPlayerPool__FindTagged(CPlayerPool *list);

/* The port (round 9 x1): the action's CAI by member. The N64 view's 0x34/0x36 are m_Mode and
 * m_ModeBefore, 0xC7 m_AnimRepeatedCount (the go animation played through), 0x110/0x114
 * m_VarBuffer[1]/[2] (Action_Setup and Action_IdleEnter clear them); on the host those offsets
 * fall elsewhere in CAI (its pointers are wider), so a used switch never left mode 1. */
void Action_GoUpdate(CGameObjectInstance *actor, ActionState *state_) {
    CAI *state = (CAI *) state_;
    CDoorIntelligence *info = &actor->header.m_pIntelligence->m_Door;
    CGame *game;
    CPlayer *player;
    s32 mode;

    if (actor->m_nTypeFlag == 0x64E && state->m_ModeBefore != state->m_Mode) {
        game = &gEngineApp_m_Game;
        player = CPlayerPool__FindTagged(&game->m_PlayerList);
        if (player != NULL && state->m_VarS32[2] == 0) {
            mode = player->m_pOptions->m_TagMode;
            if (mode == 1) {
                player->m_pOptions->m_TagMode = 0;
                player->m_pOptions->m_LastTagMode = mode;
                game->m_Settings.match.tagPhase = mode;
                state->m_VarS32[2] = mode;
            }
        }
    }
    if (state->m_AnimRepeatedCount != 0) {
        if (info->m_dwFlags & 2) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (state->m_VarS32[1] != 0) {
            CAI__SetMode(actor, state, 0);
        }
        if (actor->m_nTypeFlag == 0x64C) {
            CAI__SetMode(actor, state, 2);
        }
    }
}
void Action_DrawFade(sptr arg0, CAI *arg1, sptr arg2);

void Action_ChildVanishEnter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = Action_DrawFade;
}

#include "m2c_macros.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 9 x1): by member (N64 0x180 = m_AI.m_ModeTime, 0xD4 = m_dwFlags). Through the
 * N64 offsets the rescued child's vanish read a float and cleared bits inside other fields. */
void Action_ChildVanishUpdate(void *arg0_) {
    CGameObjectInstance *arg0 = (CGameObjectInstance *) arg0_;

    if (arg0->m_AI.m_ModeTime > 30.0f) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000 & ~0x100);
    }
}
#else
void Action_ChildVanishUpdate(void *arg0)
{
  int new_var;
  new_var = ~0x2000;
  if ((*((f32 *) (((s8 *) arg0) + 0x180))) > 30.0f)
  {
    *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0xD4))) & new_var) & (~0x100));
  }
}
#endif

void Action_EventGo(sptr arg0, sptr arg1) {
    CAI__SetMode(arg0, arg1, 1);
}

#include "structs/event.h"

void Action_EventGoRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}

void Action_EventIdle(sptr arg0, sptr arg1) {
    CAI__SetMode(arg0, arg1, 0);
}

void Action_EventIdleRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 0);
}

#include "structs/light.h"
#include "globals/engine.h"

void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void Render_QueueModelInstance(sptr anim, sptr model, s32 one, CLights1 *slot, sptr zero, s32 first, s32 second);
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* Host body: the instance's members (N64 0x14 m_pIntelligence, 0x98 m_pmtDrawMtxs, 0x110
 * m_Lights, 0x14C m_AI.m_VisBits; the AI's 0x34 m_Mode, 0x11 m_nTexture). The
 * intelligence body is asset data: its byte offsets stay. */
void Action_Draw(u8 *actorp, u8 *statep, CDrawArgs *req) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actorp;
    CAI *state = (CAI *) statep;
    u8 *info = (u8 *) actor->header.m_pIntelligence + 0x14;
    s32 first;
    s32 second;

    if (state->m_Mode == 0) {
        first = *(s8 *) (info + 0xE);
        second = *(s8 *) (info + 0xF);
    } else {
        first = *(s8 *) (info + 0x1A);
        second = *(s8 *) (info + 0x1B);
    }
    req->m_nModel = first;
    req->m_nShadowModel = first;
    state->m_nTexture = second;
    actor->m_AI.m_VisBits = 1 << req->m_nModel;
    if (req->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, state, req);
    }
    if (req->m_bDraw != 0) {
        Render_QueueModelInstance(req->m_hModel, (sptr) actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0,
                      first, second);
    }
}
#else
void Action_Draw(u8 *actor, u8 *state, CDrawArgs *req) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    s32 first;
    s32 second;

    if (*(s8 *) (state + 0x34) == 0) {
        first = *(s8 *) (info + 0xE);
        second = *(s8 *) (info + 0xF);
    } else {
        first = *(s8 *) (info + 0x1A);
        second = *(s8 *) (info + 0x1B);
    }
    req->m_nModel = first;
    req->m_nShadowModel = first;
    *(s8 *) (state + 0x11) = second;
    *(s32 *) (actor + 0x14C) = 1 << req->m_nModel;
    if (req->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, state, req);
    }
    if (req->m_bDraw != 0) {
        Render_QueueModelInstance(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0,
                      first, second);
    }
}
#endif

s32 CGameObjectInstance__DrawModelImmediate(sptr, sptr, sptr);

void Action_DrawFade(sptr arg0, CAI *arg1, sptr arg2) {
    f32 var_f1;

    var_f1 = 255.0f - (arg1->m_ModeTime * 8.5f);
    gBlendState = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    gBlendState_m_Color[3] = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (sptr) arg1, arg2);
}
