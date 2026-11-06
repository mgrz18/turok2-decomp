/* CPlayer code: CPlayer__Activate, 0x0042E9A4-0x0042EA00 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"

extern s32 gEngineApp_m_Settings_matchMode;

extern void CPlayer__NewGame(CPlayer *p, s32 arg1, s32 arg2);
extern void CAI__SetMode(void *, void *, s32);
extern void CPlayer__Warp(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void CPlayer__Activate(CPlayer *p, s32 arg1) {
    p->m_pOptions->m_bActive = 1;
    CPlayer__NewGame(p, arg1, 1);
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
    CPlayer__Warp(p, gEngineApp_m_Settings_matchMode, 0, 1);
}
