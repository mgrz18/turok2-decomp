/* Second VM module copy of func_0042EB04 (same source, linked 0x0042EA24). */
#include "common.h"
#include "structs/player.h"

extern void *func_00275F7C(void);
extern void func_00275D34(void *pool, s32 sound);

/* Stops the player's two looping sounds. */
void func_00467A24(CPlayer *p) {
    func_00275D34(func_00275F7C(), p->m_pLoopingSound_WeaponBackground);
    p->m_pLoopingSound_WeaponBackground = 0;
    func_00275D34(func_00275F7C(), p->m_pLoopingSound_WeaponFiring);
    p->m_pLoopingSound_WeaponFiring = 0;
}
