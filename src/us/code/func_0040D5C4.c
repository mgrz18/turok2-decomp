/* First-module copy of BlindOne_BossInit_2 (src/us/code/BlindOne_BossInit_2.c). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/onscreen.h"


/* AI init of the second boss: sets the level's warp id, the anim table and callbacks, starts the phase script
 * and shows its health bar ("EyeBall"). */


extern s32 gScene_m_LevelID[]; /* warp id */
extern u8 gBlindOneBossModeTable[];
extern BlindOneStep sBlindOneScript[];
extern void BlindOne_BossUpdate(void);
extern void BlindOne_BossHurt(void);
extern void BlindOne_Draw(void);
extern void BlindOne_EnterPhase(BlindOneState *state, s32 id);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 state);
extern void COnScreen__SetBoss(void *hud, CGameObjectInstance *obj, char *name);

/* Starts the boss script: its first step runs at once. */
static __inline__ void StartScript(BlindOneState *state, BlindOneStep *script, CGameObjectInstance *obj) {

    state->script = script;
    gBlindOne.boss = (struct CGameObjectInstance *) obj;
    BlindOne_EnterPhase(state, gBlindOne.script->id);
}

void BlindOne_BossInit(CGameObjectInstance *obj, CAI *ai) {
    gScene_m_LevelID[0] = 0x1CE9;
    ai->m_pModeTable = gBlindOneBossModeTable;
    ai->m_pfStep = BlindOne_BossUpdate;
    ai->m_pfHurt = BlindOne_BossHurt;
    ai->m_pfDraw = BlindOne_Draw;
    StartScript(&gBlindOne, sBlindOneScript, obj);
    obj->m_dwFlags &= ~0x10000000;
    ai->m_BlindOne.m_nPhaseHits = 0;
    ai->m_BlindOne.m_HitTimer = 0.0f;
    CAI__SetMode(obj, ai, 1);
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj, "EyeBall");
}
