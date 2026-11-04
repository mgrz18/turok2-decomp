/* CPlayer code: func_0042E91C, 0x0042E91C-0x0042E9A4 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"



extern void func_002470E8(void *, ControllerProfile *);
extern void func_00248340(CPlayer *p);

void func_0042E91C(CPlayer *p, s32 index, void *options) {
    p->m_bActive = 0;
    func_002470E8(&p->m_ContState, &D_800F5EC0[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    func_00248340(p);
}
