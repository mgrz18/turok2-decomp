/* Second-module copy of func_0040D5C4 (nonmatching/vm/func_0040D5C4.c), with its data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/script.h"

extern BlindOneState D_80130200;

/* AI init of the second boss: sets the level's warp id, the anim table and callbacks, starts the phase script
 * and shows its health bar ("EyeBall"). */


extern s32 D_80110048[]; /* warp id */
extern u8 D_800C1910[];
extern BlindOneStep D_00432260[];
extern u8 D_801100F0[];
extern void func_0040DA90(void);
extern void func_0040D9B8(void);
extern void func_0040EDB0(void);
extern void func_0040EC14(BlindOneState *state, s32 id);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 state);
extern void func_0027AD24(void *hud, CGameObjectInstance *obj, char *name);

/* Starts the boss script: its first step runs at once. */
static __inline__ void StartScript(BlindOneState *state, BlindOneStep *script, CGameObjectInstance *obj) {

    state->script = script;
    D_80130200.boss = (struct CGameObjectInstance *) obj;
    func_0040EC14(state, D_80130200.script->id);
}

void func_004465C4(CGameObjectInstance *obj, CAI *ai) {
    D_80110048[0] = 0x1CE9;
    ai->m_pModeTable = D_800C1910;
    ai->m_pfStep = func_0040DA90;
    ai->m_pfHurt = func_0040D9B8;
    ai->m_pfDraw = func_0040EDB0;
    StartScript(&D_80130200, D_00432260, obj);
    obj->m_dwFlags &= ~0x10000000;
    ai->m_VarS32[0] = 0;
    ai->m_VarF32[3] = 0.0f;
    func_00243414(obj, ai, 1);
    func_0027AD24(D_801100F0, obj, "EyeBall");
}
