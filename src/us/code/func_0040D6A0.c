#include "common.h"
#include "structs/game_object.h"

/* AI init for the module's minions (types 0x456 and others): sets the anim table and callbacks, targets the
 * current player and starts anim state 1. */

extern u8 D_800C1A90[];
extern void *D_80119890; /* current player */
extern void func_0040DF70(void);
extern void func_0040DDB4(void);
extern void func_0025E4B8(void);
extern void func_0040EEA0(void);
extern void func_0040EDB0(void);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 state);

void func_0040D6A0(CGameObjectInstance *obj, CAI *ai) {
    void *player;

    ai->m_pModeTable = D_800C1A90;
    ai->m_pfStep = func_0040DF70;
    ai->m_pfHurt = func_0040DDB4;
    ai->m_pfEffects = func_0025E4B8;
    if (obj->m_nTypeFlag == 0x456) {
        ai->m_pfDraw = func_0040EEA0;
    } else {
        ai->m_pfDraw = func_0040EDB0;
    }
    ai->m_BlindOne.m_PhaseState = 0;
    ai->m_BlindOne.m_HitTimer = 0;
    ai->m_BlindOne.m_SightDist = 0;
    ai->m_Health = 0;
    obj->m_dwFlags &= ~0x10000000;
    if (obj->m_nTypeFlag == 0x456) {
        obj->m_dwFlags |= 0x20000;
    } else {
        obj->m_dwFlags &= ~0x20000;
    }
    player = D_80119890;
    if (player != NULL) {
        ai->m_pSightTarget = player;
        ai->m_pPathTarget = player;
        ai->m_dwFlags |= 0x60000000;
    }
    func_00243414(obj, ai, 1);
    obj->m_dwFlags &= ~0x100;
}
