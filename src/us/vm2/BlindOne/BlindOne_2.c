/* Second-module copy of BlindOne_Setup (in src/us/code/BlindOne_UpdateMover.c), with its data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/onscreen.h"
#include "blindone_scene.h"
#include "globals/boss.h"
#include "protos/scene.h"

/* Second boss setup: collects its three groups of actors (types 0x454-0x456), sets their health and slots,
 * finds its two level objects, shows its health bar ("Blind") and enters the first phase of its script. */

/* Plays a sequence of bit masks: every interval seconds, each set bit sends the matching actor (bits 0-3:
 * group 1, 4-7: group 2, 8-11: group 3, rotated by offset) to an attack state. */

extern void ScriptAnimator_Start(void *slot, s32 arg1);
extern void COnScreen__SetBoss(void *hud, CGameObjectInstance *obj, char *name);

/* Steps the script: clears the movers and the actors' m_BlindOne.m_nPhaseHits, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = NULL;
    state->movers[1].seq = NULL;
    state->movers[2].seq = NULL;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    state->phase = phase;
    if (sBlindOnePhases_2[phase].init != NULL) {
        sBlindOnePhases_2[phase].init(state);
    }
}

void BlindOne_Setup_2(BlindOneState *state) {
    CGameObjectInstance *obj;
    CGameObjectInstance *actor;
    s32 i;

    state->count1 = CScene__FindInstances(&gScene, 0x454, 1, -1, state->actors1, 4);
    state->count2 = CScene__FindInstances(&gScene, 0x455, 1, -1, state->actors2, 4);
    state->count3 = CScene__FindInstances(&gScene, 0x456, 1, -1, state->actors3, 3);
    obj = state->boss;
    state->faderCount = 0;
    obj->m_AI.m_Health = obj->m_AI.m_InitialHealth = 0x3200;
    obj->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    for (i = 0; i < state->count1; i++) {
        actor = state->actors1[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x6400;
        actor->m_AI.m_BlindOne.m_nGroupIndex = i;
        actor->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    }
    for (i = 0; i < state->count2; i++) {
        actor = state->actors2[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x5000;
        actor->m_AI.m_BlindOne.m_nGroupIndex = i;
        actor->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    }
    for (i = 0; i < state->count3; i++) {
        actor = state->actors3[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x6400;
        actor->m_AI.m_BlindOne.m_nGroupIndex = i;
        actor->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    }
    for (i = 0; i < state->faderCount; i++) {
        ScriptAnimator_Start(&state->faders[i], 0);
    }
    state->minionDefA = CScene__FindIntelligence(&gScene, 1, 0x1CE9);
    state->minionDefB = CScene__FindIntelligence(&gScene, 1, 0x1CEA);
    state->toSpawn = 0;
    state->minionsAlive = 0;
    state->maxMinions = 0;
    state->alive1 = 0;
    state->alive2 = 0;
    state->alive3 = 0;
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, state->boss, "Blind");
    CScene__UnmarkGeneratorsSaved(&gScene);
    nextPhase(state);
}

#include "structs/player.h"
#include "globals/game.h"

/* Script command: waits until the current player is within 716.8 units of the origin, then steps the
 * script to the next command. */

extern f32 func_00298470(f32);

/* Advances the script by one command, stops the movers and runs the new command. */
static __inline__ void NextCommand(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    script = m->script;
    m->script = script + 1;
    cmd = script[1].id;
    m->movers[0].seq = NULL;
    m->movers[1].seq = NULL;
    m->movers[2].seq = NULL;
    for (i = 0; i < m->count1; i++) {
        m->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count2; i++) {
        m->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count3; i++) {
        m->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    m->phase = cmd;
    if (sBlindOnePhases_2[cmd].init != NULL) {
        sBlindOnePhases_2[cmd].init(m);
    }
}

void BlindOne_PhaseWaitPlayerUpdate_2(BlindOneState *m) {
    CPlayer *player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;

    if (player != NULL) {
        if (func_00298470(player->m_CharacterActor.header.m_vPos.x * player->m_CharacterActor.header.m_vPos.x + player->m_CharacterActor.header.m_vPos.y * player->m_CharacterActor.header.m_vPos.y +
                          player->m_CharacterActor.header.m_vPos.z * player->m_CharacterActor.header.m_vPos.z) < 716.8f) {
            NextCommand(m);
        }
    }
}

#include "protos/game.h"

/* Second boss phase update: spawns a minion at a random point 1228.8-1536 around the boss while the phase has
 * some left (and fewer than its maximum are alive); once none are left and fewer than 5 are alive, steps the
 * script to the next phase. */

/* Plays a sequence of bit masks: every interval seconds, each set bit sends the matching actor (bits 0-3:
 * group 1, 4-7: group 2, 8-11: group 3, rotated by offset) to an attack state. */

extern f32 Random_Blend(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void GameObject_TraceMove(CGameObjectInstance *obj, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 x, s32 y);

/* Spawns one minion around the boss when *toSpawn allows it and fewer than maxAlive are alive. */
static __inline__ void spawnMinion(BlindOneState *state, s32 *toSpawn, s32 *alive, f32 dist, s32 maxAlive,
                                   void *intelligence, CGameObjectInstance *obj, f32 angle) {
    CVector3 pos;
    CVector3 vel;
    CVector3 scale;
    sptr region;

    if (toSpawn != NULL && *toSpawn == 0) {
        return;
    }
    if (alive != NULL && maxAlive != -1 && *alive >= maxAlive) {
        return;
    }
    vel.z = vel.y = vel.x = 0.0f;
    scale.z = scale.y = scale.x = 0.5f;
    pos.x = obj->header.m_vPos.x + dist * func_002982F0(angle);
    pos.y = 0.0f;
    pos.z = obj->header.m_vPos.z + dist * func_002974C0(angle);
    GameObject_TraceMove(state->boss, pos, &pos, &region, 0, 0);
    if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x1FF, intelligence, region, pos, angle, vel, scale, alive) && toSpawn != NULL) {
        (*toSpawn)--;
    }
}

void BlindOne_PhaseMinionsUpdate_2(BlindOneState *state) {
    spawnMinion(state, &state->toSpawn, &state->minionsAlive, Random_Blend(1228.7999f, 1536.0f), state->maxMinions,
                state->minionDefA, state->boss, Random_Blend(-3.141593f, 3.141593f));
    if (state->toSpawn == 0 && state->minionsAlive < 5) {
        nextPhase(state);
    }
}

/* Second boss module: once none of its third group of actors has flag 0x100 left, steps the script to the
 * next phase. */



/* Steps the script: clears the movers and the actors' m_BlindOne.m_nPhaseHits, enters the next phase and runs its init. */
static __inline__ void nextPhase_4463A8(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = 0;
    state->movers[1].seq = 0;
    state->movers[2].seq = 0;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    state->phase = phase;
    if (sBlindOnePhases_2[phase].init != NULL) {
        sBlindOnePhases_2[phase].init(state);
    }
}

void BlindOne_PhaseGroup3Update_2(BlindOneState *state) {
    s32 done = 1;
    s32 i;

    for (i = 0; i < state->count3; i++) {
        if (state->actors3[i]->m_dwFlags & 0x100) {
            done = 0;
            break;
        }
    }
    if (done) {
        nextPhase_4463A8(state);
    }
}

#include "protos/ai.h"

/* Second boss module: plays sound 0x284F, sends the module object to anim state 1 and steps the script to
 * the next phase. */


void Sound__Play2D(s32 sound);

/* Steps the script: clears the movers and the actors' m_BlindOne.m_nPhaseHits, enters the next phase and runs its init. */
static __inline__ void nextPhase_4464C8(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = 0;
    state->movers[1].seq = 0;
    state->movers[2].seq = 0;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    state->phase = phase;
    if (sBlindOnePhases_2[phase].init != NULL) {
        sBlindOnePhases_2[phase].init(state);
    }
}

void BlindOne_PhaseBossMode1Init_2(BlindOneState *state) {
    Sound__Play2D(0x284F);
    CAI__SetMode(state->boss, &state->boss->m_AI, 1);
    nextPhase_4464C8(state);
}

#include "globals/boss_ai.h"

void BlindOne_BossUpdate_2(void *obj, CAI *ai);
void BlindOne_BossHurt_2(void *obj, CAI *ai, DamageEvent *event);
void BlindOne_EnterPhase_2(BlindOneState *m, s32 cmd);

/* Starts the boss script: its first step runs at once. */
static __inline__ void StartScript(BlindOneState *state, BlindOneStep *script, CGameObjectInstance *obj) {

    state->script = script;
    gBlindOne.boss = (struct CGameObjectInstance *) obj;
    BlindOne_EnterPhase_2(state, gBlindOne.script->id);
}
#include "structs/model.h"
void BlindOne_Draw_2(CGameObjectInstance *actor, CAI *ai, CDrawArgs *info);

void BlindOne_BossInit_2(CGameObjectInstance *obj, CAI *ai) {
    gScene_m_LevelID[0] = 0x1CE9;
    ai->m_pModeTable = gBlindOneBossModeTable;
    ai->m_pfStep = BlindOne_BossUpdate_2;
    ai->m_pfHurt = BlindOne_BossHurt_2;
    ai->m_pfDraw = BlindOne_Draw_2;
    StartScript(&gBlindOne, sBlindOneScript_2, obj);
    obj->m_dwFlags &= ~0x10000000;
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0.0f;
    CAI__SetMode(obj, ai, 1);
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj, "EyeBall");
}
void BlindOne_ActorUpdate_2(void *obj, CAI *ai);
void BlindOne_ActorHurt_2(void *obj, CAI *ai, DamageEvent *event);
extern void EnemyAI_HeadNodeCallback(void);
void BlindOne_Group3Draw_2(CGameObjectInstance *arg0, CAI *arg1, CDrawArgs *arg2);

void BlindOne_Group1Init_2(CGameObjectInstance *obj, CAI *ai) {
    void *player;

    ai->m_pModeTable = gBlindOneMinionModeTable;
    ai->m_pfStep = BlindOne_ActorUpdate_2;
    ai->m_pfHurt = BlindOne_ActorHurt_2;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (obj->m_nTypeFlag == 0x456) {
        ai->m_pfDraw = BlindOne_Group3Draw_2;
    } else {
        ai->m_pfDraw = BlindOne_Draw_2;
    }
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0;
    ai->m_BlindOne.m_SightDist = 0;
    ai->m_Health = 0;
    obj->m_dwFlags &= ~0x10000000;
    if (obj->m_nTypeFlag == 0x456) {
        obj->m_dwFlags |= 0x20000;
    } else {
        obj->m_dwFlags &= ~0x20000;
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (player != NULL) {
        ai->m_pSightTarget = player;
        ai->m_pPathTarget = player;
        ai->m_dwFlags |= 0x60000000;
    }
    CAI__SetMode(obj, ai, 1);
    obj->m_dwFlags &= ~0x100;
}

void BlindOne_Group2Init_2(CGameObjectInstance *obj, CAI *ai) {
    void *player;

    ai->m_pModeTable = gBlindOneMinionModeTable;
    ai->m_pfStep = BlindOne_ActorUpdate_2;
    ai->m_pfHurt = BlindOne_ActorHurt_2;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (obj->m_nTypeFlag == 0x456) {
        ai->m_pfDraw = BlindOne_Group3Draw_2;
    } else {
        ai->m_pfDraw = BlindOne_Draw_2;
    }
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0;
    ai->m_BlindOne.m_SightDist = 0;
    ai->m_Health = 0;
    obj->m_dwFlags &= ~0x10000000;
    if (obj->m_nTypeFlag == 0x456) {
        obj->m_dwFlags |= 0x20000;
    } else {
        obj->m_dwFlags &= ~0x20000;
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (player != NULL) {
        ai->m_pSightTarget = player;
        ai->m_pPathTarget = player;
        ai->m_dwFlags |= 0x60000000;
    }
    CAI__SetMode(obj, ai, 1);
    obj->m_dwFlags &= ~0x100;
}

void BlindOne_Group3Init_2(CGameObjectInstance *obj, CAI *ai) {
    void *player;

    ai->m_pModeTable = gBlindOneMinionModeTable;
    ai->m_pfStep = BlindOne_ActorUpdate_2;
    ai->m_pfHurt = BlindOne_ActorHurt_2;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (obj->m_nTypeFlag == 0x456) {
        ai->m_pfDraw = BlindOne_Group3Draw_2;
    } else {
        ai->m_pfDraw = BlindOne_Draw_2;
    }
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0;
    ai->m_BlindOne.m_SightDist = 0;
    ai->m_Health = 0;
    obj->m_dwFlags &= ~0x10000000;
    if (obj->m_nTypeFlag == 0x456) {
        obj->m_dwFlags |= 0x20000;
    } else {
        obj->m_dwFlags &= ~0x20000;
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (player != NULL) {
        ai->m_pSightTarget = player;
        ai->m_pPathTarget = player;
        ai->m_dwFlags |= 0x60000000;
    }
    CAI__SetMode(obj, ai, 1);
    obj->m_dwFlags &= ~0x100;
}

#include "structs/collision.h"
#include "protos/cache.h"

/* Damage callback of the second boss: outside phase 9 it only flinches (anim state 6); in phase 9 the damage
 * is taken, a hit effect plays at most every 2.25 s, and at zero health it dies (anim state 9). */

extern void ScriptAnimator_SetScript(void *dst, void *src);

void BlindOne_BossHurt_2(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state;
    s32 health;

    if (ai->m_Health != 0 && ai->m_Mode != 1) {
        state = &gBlindOne;
        if (state->phase != 9) {
            ai->m_AnimRepeatedCount = 0;
            CAI__SetMode(obj, ai, 6);
        } else {
            health = ai->m_Health - event->damage;
            if (health < 0) {
                health = 0;
            }
            ai->m_Health = health;
            if (ai->m_BlindOne.m_HitTimer == 0.0f) {
                ai->m_BlindOne.m_HitTimer = 2.25f;
                ScriptAnimator_SetScript(&state->faders[0], sBlindOneHitScript_2);
            }
            if (ai->m_Health == 0) {
                CAI__SetMode(obj, ai, 9);
                CCache__SetFrameDelay(8);
            }
        }
    }
}

#include "globals/engine.h"
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */


void Animator_Update(void *slot);
void BlindOne_UpdateMover_2(BlindOneMover *mover);
s32 BlindOne_GetHealthPercent_2(BlindOneState *state);
void COnScreen__SetBossPercent(void *hud, s32 value);

void BlindOne_BossUpdate_2(void *obj, CAI *ai) {
    BlindOneState *state = &gBlindOne;
    s32 i;

    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_BlindOne.m_HitTimer = MAX(ai->m_BlindOne.m_HitTimer - gFrameIncrement, 0.0f);
    if (sBlindOnePhases_2[state->phase].update != NULL) {
        sBlindOnePhases_2[state->phase].update(state);
    }
    for (i = 0; i < state->faderCount; i++) {
        Animator_Update(&state->faders[i]);
    }
    BlindOne_UpdateMover_2(&state->movers[0]);
    BlindOne_UpdateMover_2(&state->movers[1]);
    BlindOne_UpdateMover_2(&state->movers[2]);
    COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, BlindOne_GetHealthPercent_2(state));
}

#include "m2c_macros.h"

void BlindOne_BossMode0Enter_2(void) {

}

void BlindOne_BossMode0Update_2(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}

void BlindOne_BossMode1Enter_2(void) {

}

void BlindOne_BossMode1Update_2(void) {

}

void BlindOne_BossMode2Enter_2(void) {

}

void BlindOne_BossMode2Update_2(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}

f32 Random_Blend(f32 arg0, f32 arg1);

void BlindOne_BossMode3Enter_2(CGameObjectInstance *obj, CAI *ai) {
    ai->m_ConditionTimer = Random_Blend(15.0f, 75.0f);
}

void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);

void BlindOne_BossMode3Update_2(CGameObjectInstance *actor, CAI *ai) {
    CAITargets targets;

    AI_UpdateTargets(actor, ai, &targets, 0, 1);
    if (ai->m_ModeTime > ai->m_ConditionTimer) {
        CAI__SetMode(actor, ai, 5);
    }
}

void BlindOne_BossMode4Enter_2(void) {

}

void BlindOne_BossMode4Update_2(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 3);
    }
}

void BlindOne_BossMode5Enter_2(void) {

}

void BlindOne_BossMode5Update_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 3);
    }
}

void BlindOne_BossFlinchEnter_2(void) {

}

void BlindOne_BossFlinchUpdate_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount >= 7) {
        CAI__SetMode(obj, ai, 7);
    }
}

void BlindOne_BossMode7Enter_2(void) {

}

void BlindOne_BossMode7Update_2(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}

void BlindOne_BossMode8Enter_2(void) {

}

void BlindOne_BossMode8Update_2(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}

void BlindOne_BossDeathEnter_2(void) {
    Sound__Play2D(0x284E);
}

void BlindOne_BossDeathUpdate_2(void) {

}

void BlindOne_ActorHurt_2(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state = &gBlindOne;
    s32 health;

    if (ai->m_Health != 0) {
        health = ai->m_Health - event->damage;
        if (health < 0) {
            health = 0;
        }
        ai->m_Health = health;
        if (ai->m_BlindOne.m_HitTimer == 0.0f) {
            ai->m_BlindOne.m_HitTimer = 2.25f;
            ScriptAnimator_SetScript(&state->faders[ai->m_BlindOne.m_nFader], sBlindOneHitScript_2);
        }
        if (ai->m_Health == 0) {
            CAI__SetMode(obj, ai, 6);
        }
    }
}

void BlindOne_ActorInit_2(CGameObjectInstance *obj, CAI *ai) {
    void *player;

    ai->m_pModeTable = gBlindOneMinionModeTable;
    ai->m_pfStep = BlindOne_ActorUpdate_2;
    ai->m_pfHurt = BlindOne_ActorHurt_2;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (obj->m_nTypeFlag == 0x456) {
        ai->m_pfDraw = BlindOne_Group3Draw_2;
    } else {
        ai->m_pfDraw = BlindOne_Draw_2;
    }
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0;
    ai->m_BlindOne.m_SightDist = 0;
    ai->m_Health = 0;
    obj->m_dwFlags &= ~0x10000000;
    if (obj->m_nTypeFlag == 0x456) {
        obj->m_dwFlags |= 0x20000;
    } else {
        obj->m_dwFlags &= ~0x20000;
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (player != NULL) {
        ai->m_pSightTarget = player;
        ai->m_pPathTarget = player;
        ai->m_dwFlags |= 0x60000000;
    }
    CAI__SetMode(obj, ai, 1);
    obj->m_dwFlags &= ~0x100;
}

/* Step callback of the Blind One's actors: while alive they target the
 * current player, run their mode's callback and count the hit flash down. */
void BlindOne_ActorUpdate_2(void *obj, CAI *ai) {
    if (ai->m_Health != 0 && gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead != 0) {
        ai->m_pSightTarget = (struct CGameObjectInstance *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        ai->m_pPathTarget = (struct CGameObjectInstance *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        ai->m_pTargetOverride = (struct CGameObjectInstance *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        ai->m_TargetOverrideTime = 150.0f;
    }
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_BlindOne.m_HitTimer = (ai->m_BlindOne.m_HitTimer - gFrameIncrement < 0.0f) ? 0.0f : ai->m_BlindOne.m_HitTimer - gFrameIncrement;
}

#include "structs/vector.h"

void BlindOne_ActorAppearEnter_2(CGameObjectInstance *actor, CAI *spawn) {
    BlindOneState *stats;

    spawn->m_HeadTilt = 0;
    spawn->m_HeadTurn = 0;
    spawn->m_Health = spawn->m_InitialHealth;
    actor->header.m_vPos = spawn->m_vInitialPos;
    actor->header.m_pCurrentRegion = spawn->m_pInitialRegion;
    stats = &gBlindOne;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            stats->alive1++;
            break;
        case 0x455:
            stats->alive2++;
            break;
        case 0x456:
            stats->alive3++;
            break;
    }
}

void BlindOne_ActorAppearUpdate_2(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 2);
    }
}

void BlindOne_ActorMode1Enter_2(void) {

}

void BlindOne_ActorMode1Update_2(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}

void BlindOne_ActorAimEnter_2(void) {

}

extern void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnHeadToTarget(CGameObjectInstance *actor, CAI *ai, f32 angle);
extern void Angle_Wrap(f32 *angle); /* wraps an angle */

void BlindOne_ActorAimUpdate_2(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    f32 delta;
    f32 rate;
    f32 range;
    f32 turn;
    f32 half;
    f32 sum;

    AI_UpdateTargets(actor, ai, &t, 0, 1);
    ai->m_BlindOne.m_SightDist = t.m_Sight.m_FlatDist;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            rate = 0.06981318f;
            range = 6.283186f;
            AI_TurnHeadToTarget(actor, ai, -1.5707965f);
            ai->m_HeadTurn = 0;
            break;
        case 0x455:
            rate = 0.06981318f;
            range = 0.78539824f;
            break;
        case 0x456:
            rate = 0.06981318f;
            range = 0.52359885f;
            break;
        default:
            range = rate = 0.0f;
            break;
    }
    delta = actor->m_RotY - ai->m_InitialRotY;
    Angle_Wrap(&delta);
    turn = t.m_Sight.m_Angle;
    if (range < 6.283186f) {
        half = range * 0.5f;
        sum = delta + turn;
        if (sum > half) {
            turn = half - delta;
        } else if (sum < -half) {
            turn = -half - delta;
        }
    }
    AI_TurnClamped(actor, ai, rate, turn);
}

void BlindOne_ActorAttack3Enter_2(void) {

}

void BlindOne_ActorAttack3Update_2(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 2);
    }
}

void BlindOne_ActorAttack4Enter_2(void) {

}

void BlindOne_ActorAttack4Update_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 2);
    }
}

void BlindOne_ActorAttack5Enter_2(void) {

}

void BlindOne_ActorAttack5Update_2(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 2);
    }
}

void BlindOne_ActorDeathEnter_2(CGameObjectInstance *actor, CAI *ai) {
    BlindOneState *counters = &gBlindOne;
    CGameObjectInstance *next;

    if (counters->boss->m_AI.m_Mode != 1) {
        CAI__SetMode(counters->boss, &counters->boss->m_AI, 8);
    }
    ai->m_BlindOne.m_nPhaseHits++;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            counters->alive1--;
            break;
        case 0x455:
            counters->alive2--;
            break;
        case 0x456:
            counters->alive3--;
            next = counters->actors3[(ai->m_BlindOne.m_nGroupIndex + 1) % counters->count3];
            if (next->m_AI.m_BlindOne.m_nPhaseHits == 0) {
                CAI__SetMode(next, &next->m_AI, 0);
            }
            break;
    }
}

void BlindOne_ActorDeathUpdate_2(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}

s32 Cinema_RequestById_2(s32);

void BlindOne_PhaseStartCinemaInit_2(void) {
    Cinema_RequestById_2(0x15E);
}

extern s32 Cinema_IsIdle(BlindOneState *m);

void BlindOne_PhaseWaitCinemaUpdate_2(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    if (Cinema_IsIdle(m)) {
        script = m->script;
        m->script = script + 1;
        cmd = script[1].id;
        m->movers[0].seq = NULL;
        m->movers[1].seq = NULL;
        m->movers[2].seq = NULL;
        for (i = 0; i < m->count1; i++) {
            m->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
        }
        for (i = 0; i < m->count2; i++) {
            m->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
        }
        for (i = 0; i < m->count3; i++) {
            m->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
        }
        m->phase = cmd;
        if (sBlindOnePhases_2[cmd].init != NULL) {
            sBlindOnePhases_2[cmd].init(m);
        }
    }
}

/* Only what this reads and writes. */


void BlindOne_PhaseMinionsInit_2(BlindOneState *arg0) {
    CVector3 *temp_v0;

    temp_v0 = arg0->script;
    arg0->toSpawn = (s32) temp_v0->y;
    arg0->maxMinions = (s32) temp_v0->z;
    CScene__MarkGeneratorsSaved(&gScene);
}

static __inline__ void StartTimer(BlindOneState *m, void *proc, f32 seconds) {
    BlindOneMover *t;

    m->movers[0].seq = proc;
    t = &m->movers[0];
    t->pos = 0;
    t->timer = 0;
    t->step = 0;
    t->offset = 0;
    t->interval = seconds * 15.0f;
}

void BlindOne_PhaseGroup1Init_2(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count1; i++) {
        CAI__SetMode(m->actors1[i], &m->actors1[i]->m_AI, 0);
    }
    StartTimer(m, sBlindOneMoverSeq1_2, m->script->seconds);
    CScene__MarkGeneratorsSaved(&gScene);
}

void BlindOne_PhaseGroup1Update_2(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    if (m->alive1 == 0) {
        script = m->script;
        m->script = script + 1;
        cmd = script[1].id;
        m->movers[0].seq = NULL;
        m->movers[1].seq = NULL;
        m->movers[2].seq = NULL;
        for (i = 0; i < m->count1; i++) {
            m->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
        }
        for (i = 0; i < m->count2; i++) {
            m->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
        }
        for (i = 0; i < m->count3; i++) {
            m->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
        }
        m->phase = cmd;
        if (sBlindOnePhases_2[cmd].init != NULL) {
            sBlindOnePhases_2[cmd].init(m);
        }
    }
}

static __inline__ void StartTimer_44772C(BlindOneState *m, s32 n, void *proc, f32 seconds) {
    BlindOneMover *t;

    m->movers[n].seq = proc;
    t = &m->movers[n];
    t->pos = 0;
    t->timer = 0;
    t->step = 0;
    t->offset = 0;
    t->interval = seconds * 15.0f;
}

void BlindOne_PhaseGroup2Init_2(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count2; i++) {
        CAI__SetMode(m->actors2[i], &m->actors2[i]->m_AI, 0);
    }
    StartTimer_44772C(m, 1, sBlindOneMoverSeq2_2, m->script->seconds);
    CScene__MarkGeneratorsSaved(&gScene);
}

void BlindOne_PhaseGroup2Update_2(BlindOneState *m) {
    if (m->alive2 == 0) {
        NextCommand(m);
    }
}

static __inline__ void StartTimer_4478BC(BlindOneState *m, s32 n, void *proc, f32 seconds) {
    BlindOneMover *t;

    m->movers[n].seq = proc;
    t = &m->movers[n];
    t->pos = 0;
    t->timer = 0;
    t->step = 0;
    t->offset = 0;
    t->interval = seconds * 15.0f;
}

void BlindOne_PhaseGroup3Init_2(BlindOneState *m) {
    CAI__SetMode(m->actors3[0], &m->actors3[0]->m_AI, 0);
    StartTimer_4478BC(m, 2, sBlindOneMoverSeq3_2, m->script->seconds);
    CScene__MarkGeneratorsSaved(&gScene);
}

static __inline__ void StartTimer_447930(BlindOneState *m, s32 n, void *proc, f32 seconds) {
    BlindOneMover *t;

    m->movers[n].seq = proc;
    t = &m->movers[n];
    t->pos = 0;
    t->timer = 0;
    t->step = 0;
    t->offset = 0;
    t->interval = seconds * 15.0f;
}

void BlindOne_PhaseBossMode0Init_2(BlindOneState *m) {
    CAI__SetMode(m->boss, &m->boss->m_AI, 0);
    NextCommand(m);
}

/* host body (round 10 y2): gBlindOne.boss by member and its CAI (N64 + 0x140): the boss pointer
 * read as an s32 crashed CAI__SetMode as the vulnerable phase began */
void BlindOne_PhaseVulnerableInit_2(BlindOneState *arg0) {
    CAI__SetMode(arg0->boss, &arg0->boss->m_AI, 0);
}

void BlindOne_PhaseVulnerableUpdate_2(BlindOneState *m) {
    if (m->boss->m_AI.m_Health == 0) {
        NextCommand(m);
    }
}

#include "structs/game.h"

extern void CPlayerPool__SetInvincibility(void *players, f32 value);

void BlindOne_PhaseVictoryInit_2(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    CScene__UnmarkGeneratorsSaved(&gScene);
}

#include "protos/player.h"

s32 CPlayer__Warp(sptr, s32, s32, s32);
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
/* host body (round 10 y2): the boss's m_AI.m_ModeTime (N64 + 0x180) and the player's
 * m_Data.m_bBlindBossComplete (N64 + 0x9E4) by member */
void BlindOne_PhaseVictoryUpdate_2(void **arg0)
{
  CGameObjectInstance *boss = (CGameObjectInstance *) *arg0;
  CPlayer *player;

  if (120.0f < boss->m_AI.m_ModeTime)
  {
    player = (CPlayer *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (player != NULL)
    {
      player->m_Data.m_bBlindBossComplete = 1;
      gEngineApp_m_Game.m_Settings.unlockedCheats |= 0x120;
      CPlayer__Warp((sptr) player, 0x1387, 2, 0);
    }
  }
}
#else
void BlindOne_PhaseVictoryUpdate_2(void **arg0)
{
  f32 new_var3;
  void *temp_v0;
  void *new_var2;
  GameSettings *temp_v1;
  s32 *new_var;
  new_var3 = *((f32 *) (((s8 *) (*arg0)) + 0x180));
  if (120.0f < new_var3)
  {
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    new_var2 = temp_v0;
    new_var = (s32 *) (((s8 *) new_var2) + 0x9E4);
    if (temp_v0 != 0)
    {
      temp_v1 = &gEngineApp_m_Game.m_Settings;
      *new_var = 1;
      temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 0x120);
      CPlayer__Warp((sptr) temp_v0, 0x1387, 2, 0);
    }
  }
}
#endif

void BlindOne_StartScript_2(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = sBlindOneScript_2;
    BlindOne_EnterPhase_2(m, sBlindOneScript_2[0].id);
}

/* Starts script command `cmd`: stops the movers, clears every member's flag and runs the command. */
void BlindOne_EnterPhase_2(BlindOneState *m, s32 cmd) {
    s32 i;

    m->movers[0].seq = NULL;
    m->movers[1].seq = NULL;
    m->movers[2].seq = NULL;
    for (i = 0; i < m->count1; i++) {
        m->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count2; i++) {
        m->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count3; i++) {
        m->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    m->phase = cmd;
    if (sBlindOnePhases_2[cmd].init != NULL) {
        sBlindOnePhases_2[cmd].init(m);
    }
}

void BlindOne_UpdateFight_2(BlindOneState *arg0)
{
  void (*temp_v0)(void);
  s32 var_s0;
  s32 var_s1;
  temp_v0 = (void (*)(void)) sBlindOnePhases_2[arg0->phase].update;
  if (temp_v0 != 0)
  {
    /* ABI (docs/port/ABI.md section 5): MIPS passes arg0 through to the phase update (a0) */
    ((void (*)(BlindOneState *))temp_v0)(arg0);
  }
  var_s0 = 0;
  if (arg0->faderCount > 0)
  {
    var_s1 = 0x3C;      /* faders[i] */
    do
    {
      Animator_Update((void *) ((u8 *) arg0 + var_s1));
      var_s0 += 1;
      var_s1 += sizeof(ScriptAnimator);
      var_s0++;
      var_s0--;
    }
    while (var_s0 < arg0->faderCount);
  }
  BlindOne_UpdateMover_2(&arg0->movers[0]);
  BlindOne_UpdateMover_2(&arg0->movers[1]);
  BlindOne_UpdateMover_2(&arg0->movers[2]);
  COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, BlindOne_GetHealthPercent_2(arg0));
}

void func_00447D8C(s32 *arg0) {
    *arg0 = 0;
}

/* Starts a Blind One mover on a sequence of masks, one every interval frames. */
void BlindOne_StartMover_2(BlindOneMover *mover, s32 *seq, f32 interval) {
    mover->seq = seq;
    mover->pos = 0;
    mover->interval = interval;
    mover->timer = 0;
    mover->step = 0;
    mover->offset = 0;
}

#include "structs/light.h"
#include "structs/twin_blindone.h"

void ScriptAnimator_Apply(ScriptAnimator *light);
void ScreenTint_UpdateMode(void);

void BlindOne_Draw_2(CGameObjectInstance *actor, CAI *ai, CDrawArgs *info) {
    ScriptAnimator_Apply(&gBlindOne.faders[ai->m_BlindOne.m_nFader]);
    ScreenTint_UpdateMode();
    switch (actor->m_nTypeFlag) {
        case 0x450:
            if (ai->m_Mode == 9) {
                info->m_nModel = 1;
            }
            break;
        case 0x455:
            if (ai->m_Mode == 6) {
                info->m_nModel = 1;
            }
            break;
    }
    Render_DrawModel(info->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0, info->m_nModel, actor->m_AI.m_nTexture);
    gBlendState = 0;
}

s32 CGameObjectInstance__DrawModel(sptr, sptr, sptr);

void BlindOne_Group3Draw_2(CGameObjectInstance *arg0, CAI *arg1, CDrawArgs *arg2)
{
  BlindOneState *state;
  state = &gBlindOne;
  CGameObjectInstance__DrawModel((sptr) arg0, (sptr) arg1, (sptr) arg2);
  if (arg1->m_Health != 0)
  {
    ScriptAnimator_Apply(&state->faders[arg1->m_BlindOne.m_nFader]);
    ScreenTint_UpdateMode();
    Render_DrawModel(arg2->m_hModel, arg0->m_pmtDrawMtxs, 1, &arg0->m_Lights[gEvenOdd], 0, 1, arg0->m_AI.m_nTexture);
  }
  gBlendState = 0;
}

void BlindOne_NextPhase_2(BlindOneState *m) {
    NextCommand(m);
}

extern void GameObject_TraceMove(CGameObjectInstance *actor, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 flagA, s32 flagB);

/* Spawns an actor at `dist` from `origin` in direction `angle`, if the budget allows it. */
void BlindOne_SpawnMinion_2(CGameObjectInstance **self, s32 *budget, s32 *spawned, s32 max, void *intelligence, CGameObjectInstance *origin, f32 dist,
                   f32 angle) {
    CVector3 pos;
    CVector3 vel;
    CVector3 scale;
    sptr region;

    if (budget != NULL && *budget == 0) {
        return;
    }
    if (spawned != NULL && max != -1 && *spawned >= max) {
        return;
    }
    vel.x = 0.0f;
    vel.y = 0.0f;
    vel.z = 0.0f;
    scale.x = 0.5f;
    scale.y = 0.5f;
    scale.z = 0.5f;
    pos.x = origin->header.m_vPos.x + dist * func_002982F0(angle);
    pos.y = 0.0f;
    pos.z = origin->header.m_vPos.z + dist * func_002974C0(angle);
    GameObject_TraceMove(*self, pos, &pos, &region, 0, 0);
    if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x1FF, intelligence, region, pos, angle, vel, scale, spawned) != NULL &&
        budget != NULL) {
        (*budget)--;
    }
}

/* Sums `value` over the group, and separately over the members still alive with flag 0x100. */
void BlindOne_SumGroupHealth_2(void *m, s32 count, CGameObjectInstance **group, s32 *total, s32 *active) {
    CGameObjectInstance *a;
    s32 i;

    for (i = 0; i < count; i++) {
        a = group[i];
        *total += a->m_AI.m_InitialHealth;
        if (a->m_AI.m_Health != 0 && (a->m_dwFlags & 0x100)) {
            *active += a->m_AI.m_InitialHealth;
        }
    }
}
