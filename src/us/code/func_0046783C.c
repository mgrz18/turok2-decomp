/* Second VM module copy of CPlayer__Construct (same source, linked 0x0042E83C). */
#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"



extern void CPlayerContState__Init(void *, ControllerProfile *);
extern void CPlayer__Reset(CPlayer *p);

void CPlayer__Construct_2(CPlayer *p, s32 index, void *options) {
    p->m_bActive = 0;
    CPlayerContState__Init(&p->m_ContState, &gControllers[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    CPlayer__Reset(p);
}
