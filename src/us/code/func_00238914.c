#include "structs/game_object.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_2/802661FC_2.CPP, func_80267D70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


typedef struct Player {
    char pad0[0xB18];
    CVector3 requestedPosition;  /* 0xB18 (Rage Wars 0x774) */
} Player;


extern void func_00258734(void *actor, s32 flagsAddr, s32 arg2);

void func_00238914(s32 arg0, void *arg1, s32 arg2, CVector3 arg3, s32 arg6)
{
    s32 t6 = arg6;
    Player *player = ((Player *)((CGameObjectInstance *)arg1)->m_AI.m_pParent);
    player->requestedPosition = arg3;
    do { } while (0);
    func_00258734(arg1, (s32)&((CGameObjectInstance *)arg1)->m_AI.m_dwFlags, t6);
}
