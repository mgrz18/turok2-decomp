/* Second VM module copy of CPlayer__Activate (same source, linked 0x0042E8C4). */
#include "common.h"
#include "structs/player.h"
#include "globals/game.h"
#include "protos/ai.h"


extern void CPlayer__NewGame_2(CPlayer *p, s32 arg1, s32 arg2);
extern void CPlayer__Warp(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void CPlayer__Activate_2(CPlayer *p, s32 arg1) {
    p->m_pOptions->m_bActive = 1;
    CPlayer__NewGame_2(p, arg1, 1);
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
    CPlayer__Warp(p, gEngineApp_m_Settings.matchMode, 0, 1);
}
