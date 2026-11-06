/* First-module copy of BlindOne_BossInit_2 (src/us/code/BlindOne_BossInit_2.c). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOneState D_80130200;

/* AI init of the second boss: sets the level's warp id, the anim table and callbacks, starts the phase script
 * and shows its health bar ("EyeBall"). */


extern s32 D_80110048[]; /* warp id */
extern u8 D_800C1910[];
extern BlindOneStep D_00432340[];
extern u8 D_801100F0[];
extern void BlindOne_BossUpdate(void);
extern void BlindOne_BossHurt(void);
extern void BlindOne_Draw(void);
extern void BlindOne_EnterPhase(BlindOneState *state, s32 id);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 state);
extern void COnScreen__SetBoss(void *hud, CGameObjectInstance *obj, char *name);

/* Starts the boss script: its first step runs at once. */
static __inline__ void StartScript(BlindOneState *state, BlindOneStep *script, CGameObjectInstance *obj) {

    state->script = script;
    D_80130200.boss = (struct CGameObjectInstance *) obj;
    BlindOne_EnterPhase(state, D_80130200.script->id);
}

void BlindOne_BossInit(CGameObjectInstance *obj, CAI *ai) {
    D_80110048[0] = 0x1CE9;
    ai->m_pModeTable = D_800C1910;
    ai->m_pfStep = BlindOne_BossUpdate;
    ai->m_pfHurt = BlindOne_BossHurt;
    ai->m_pfDraw = BlindOne_Draw;
    StartScript(&D_80130200, D_00432340, obj);
    obj->m_dwFlags &= ~0x10000000;
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0.0f;
    CAI__SetMode(obj, ai, 1);
    COnScreen__SetBoss(D_801100F0, obj, "EyeBall");
}
