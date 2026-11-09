#include "structs/game_object.h"
#include "structs/player.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_2/802661FC_2.CPP, func_80267D70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */



extern void WeaponActor_FireCallback(void *actor, s32 flagsAddr, s32 arg2);

void ImpactEvent_SetWeaponFirePos(s32 arg0, void *arg1, s32 arg2, CVector3 arg3, s32 arg6)
{
    s32 t6 = arg6;
    CPlayer *player = ((CPlayer *)((CGameObjectInstance *)arg1)->m_AI.m_pParent);
    player->m_vWeaponFirePos = arg3;
    do { } while (0);
    WeaponActor_FireCallback(arg1, (sptr)&((CGameObjectInstance *)arg1)->m_AI.m_dwFlags, t6);
}
