#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__AdvanceAnimation(CGameObjectInstance *);
s32 CGameObjectInstance__SetAnimByAction(CGameObjectInstance *, s32, s32);

/* ai: the CAI (protos/ai.h declares it void *) */
void AI_PlayAnim(sptr arg0, void *ai, s32 arg2) {
    CAI *arg1 = ai;

    arg1->m_ModeLastFrame = -1;
    arg1->m_AnimRepeatedCount = 0;
    arg1->m_VarBuffer[0] = arg2;
    CGameObjectInstance__SetAnimByAction((CGameObjectInstance *) arg0, arg2, 1);
    CGameObjectInstance__AdvanceAnimation((CGameObjectInstance *) arg0);
}

#include "globals/ai.h"
#include "globals/game.h"

void InteractiveAnim_Step(void *arg0, CAI *arg1);
s32 Cinema_GetScriptId(void);
#include "structs/model.h"
void InteractiveAnim_DrawAllModels(u8 *actor, u8 *state, CDrawArgs *req);

void InteractiveAnim_Setup(CGameObjectInstance *actor, CAI *state) {
    state->m_pModeTable = gInteractiveAnimModeTable;
    state->m_pfStep = InteractiveAnim_Step;
    actor->m_dwFlags |= 0x1000000;
    actor->m_dwFlags |= 0x2000000;
    actor->m_dwFlags |= 0x20000;
    if (gEngineApp_m_Settings.multiplayer == 0) {
        actor->m_dwFlags |= 0x10000000;
    }
    if (actor->m_nTypeFlag == 0x453 && Cinema_GetScriptId() == 0x6F) {
        state->m_pfDraw = InteractiveAnim_DrawAllModels;
    }
}

#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

/* Typed on CGameObjectInstance/CAI (m2c's offsets 0x14/0xD4 and the AI's 0x10C-0x114 are the
 * N64 layout): the AI's first variable takes the word after the intelligence's type flags. */
void InteractiveAnim_Advance(CGameObjectInstance *arg0, CAI *arg1)
{
  int new_var;
  arg1->m_VarS32[0] = ((s32 *) &arg0->header.m_pIntelligence->m_dwTypeFlags)[1];
  CAI__SetMode(arg0, arg1, 0);
  arg1->m_VarF32[1] = 1.0f;
  arg1->m_VarF32[2] = 1.0f;
  if (CScene__GetInstanceSavedBit((&gScene), arg0) == 1)
  {
    new_var = ~0x2000;
    arg0->m_dwFlags = (arg0->m_dwFlags & new_var) & (~0x100);
  }
}

/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void InteractiveAnim_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}

s32 InteractiveAnim_GetAnim(sptr arg0, CAI *arg1) {
    return arg1->m_VarBuffer[0];
}

#include "globals/enemy.h"

void InteractiveAnim_Update(CGameObjectInstance *obj, CAI *ai) {
    s32 temp_a1;

    if (((u32) ((u16) ai->m_AnimType - 0xFA0) < 0x401U) && (ai->m_AnimRepeatedCount != 0)) {
        temp_a1 = AI_PickAnimFromTable(obj, ai, sEnemyAnimIdle_AF6F0, -1);
        ai->m_ModeLastFrame = -1;
        ai->m_AnimRepeatedCount = 0;
        ai->m_VarBuffer[0] = temp_a1;
        CGameObjectInstance__SetAnimByAction(obj, temp_a1, 1);
        CGameObjectInstance__AdvanceAnimation(obj);
    }
}

#include "structs/event.h"

/* The mode parameter this AI callback is handed: only the field it reads. */

void InteractiveAnim_EventPlay(CGameObjectInstance *arg0, sptr arg1, LevelEvent *arg2) {
    AI_PlayAnim(arg0, arg1, (s32) arg2->param);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    CGameObjectInstance__AdvanceAnimation((CGameObjectInstance *) arg0);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}

void InteractiveAnim_EventHide(CGameObjectInstance *arg0)
{
  int new_var;
  new_var = ~0x2000;
  arg0->m_dwFlags = (s32) ((arg0->m_dwFlags & new_var) & (~0x100));
  CScene__SetInstanceSavedBit((&gScene), arg0, 1);
}

void InteractiveAnim_EventHideRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}

s32 Cinema_PathPlaying();                                /* extern */

void InteractiveAnim_EventShow(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
    CGameObjectInstance__AdvanceAnimation((CGameObjectInstance *) arg0);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}

void InteractiveAnim_EventShowRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 0);
}

s32 Cinema_RequestForObject(sptr, sptr);

void InteractiveAnim_EventPlayCinema(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    Cinema_RequestForObject(arg0, arg2);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}

void InteractiveAnim_EventPlayCinemaRemote(s32 arg0) {
    Cinema_RequestForObject(0, arg0);
}

typedef struct {
    /* 0x0 */ u8 pad0[7];
    /* 0x7 */ u8 value;
} ByteArg;

void InteractiveAnim_EventSetModel(CGameObjectInstance *arg0, sptr arg1, ByteArg *arg2) {
    arg0->header.m_pIntelligence->m_nModel = (u8) arg2->value;
    arg0->header.m_pIntelligence->m_nShadowModel = (u8) arg2->value;
}

#include "structs/light.h"
#include "globals/engine.h"
#include "protos/game.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

s32 *CISet__GetBlock(sptr base, s32 index);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);

void InteractiveAnim_DrawAllModels(u8 *actor, u8 *state, CDrawArgs *req) {
    s32 i;
    s32 count;

    count = *CISet__GetBlock(*(sptr *) req->m_hModel, 2); /* the handle holds a pointer */
    for (i = 0; i < count; i++) {
        req->m_nModel = i;
        req->m_nShadowModel = i;
        CGameObjectInstance__DrawShadow(actor, state, req);
#ifdef TARGET_PORT
        /* the instance's members: m_pmtDrawMtxs, m_Lights, m_AI.m_nTexture */
        Render_DrawModel(req->m_hModel, (sptr) ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                         &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, i,
                         ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
#else
        Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0, i,
                      *(s8 *) (actor + 0x151));
#endif
    }
}
