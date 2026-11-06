/* Second-module twin of src/us/code/MotherBoss_StartPhase2Cinema.c (data 0xE0 lower; MotherBoss_OnPhase2CinemaDone is the same link address). */
/* gMotherBoss_phase/D0/D4/EC are fields of gMotherBoss (phase, activeMask, unk664, counter) that the ROM addresses
 * through their own symbols (as arrays: their loads are reloaded after the ai store). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Module phase 2 entry: resets the actors' blend values and the fade timer, sets up phase 2 (timers, anim,
 * slot values), resets its active slots unless ai->m_Mode is 0x13, and queues MotherBoss_OnPhase2CinemaDone. */


extern s32 gMotherBoss_phase[]; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* gMotherBoss.phase */
extern s32 gMotherBoss_activeMask[]; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* gMotherBoss.activeMask */
extern s32 gMotherBoss_liveMask[]; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* gMotherBoss.liveMask */
extern s32 gMotherBoss_counter[]; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* gMotherBoss.counter */
extern f32 sMotherBossFadeTimer; /* fade timer (func_0040A17C) */
extern s32 sMotherBossCinemaStarted;
extern s32 sMotherBossPhaseVisBits_2[];
extern s32 sMotherBossPhaseActiveMasks_2[]; /* active slot mask per phase */
extern s32 sMotherBossPhaseLiveMasks_2[];
extern u8 sMotherBossSpotResetScript_2[];
extern void ScriptAnimator_SetScript(void *dst, void *src);
extern void MotherBoss_OnPhase2CinemaDone_2(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}

static __inline__ void enterPhase2(MotherBossState *s, CAI *ai) {
    s32 mask;

    s->minRadius = 0x200;
    s->phase = 2;
    s->maxRadius = 0x600;
    gMotherBoss_counter[0] = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits_2[gMotherBoss_phase[0]];
    gMotherBoss_liveMask[0] = sMotherBossPhaseLiveMasks_2[gMotherBoss_phase[0]];
    mask = sMotherBossPhaseActiveMasks_2[gMotherBoss_phase[0]];
    sMotherBossFadeTimer = 0.0f;
    sMotherBossCinemaStarted = 0;
    s->totalHealth = s->health = s->slots[13].health = 0x1F400;
    gMotherBoss_activeMask[0] = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(gMotherBoss_activeMask[0], sMotherBossSpotResetScript_2);
    }
}

void MotherBoss_StartPhase2Cinema_2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 1.0f;
        actor->m_AI.m_Mother.m_PulseScale = 0.0f;
    }
    enterPhase2(&gMotherBoss, ai);
    Cinema_SetNextCallbacks(0xDC, MotherBoss_OnPhase2CinemaDone_2, NULL);
}
