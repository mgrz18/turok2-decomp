#include "common.h"
#include "structs/game_object.h"

/* The controller of a boss fight (Owner00403430 in func_00403430): it holds the boss at 0x220. */
typedef struct {
    /* 0x000 */ u8 pad0[0x220];
    /* 0x220 */ CGameObjectInstance *boss;
} BossOwner;

s32 func_002666B0(s32, s32, s32);           /* extern */

void func_0040340C(BossOwner *arg0, s32 arg1) {
    CGameObjectInstance *temp_a0;

    temp_a0 = arg0->boss;
    func_002666B0((s32) temp_a0, (s32) &temp_a0->m_AI, arg1);
}
