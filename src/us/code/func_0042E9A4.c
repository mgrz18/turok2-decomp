/* CPlayer code: func_0042E9A4, 0x0042E9A4-0x0042EA00 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"

extern s32 D_8011ACA8;

extern void func_0042DC18(CPlayer *p, s32 arg1, s32 arg2);
extern void func_00243414(void *, void *, s32);
extern void func_00248BA8(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void func_0042E9A4(CPlayer *p, s32 arg1) {
    p->m_pOptions->m_bActive = 1;
    func_0042DC18(p, arg1, 1);
    func_00243414(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
    func_00248BA8(p, D_8011ACA8, 0, 1);
}
