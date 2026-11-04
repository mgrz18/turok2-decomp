#include "common.h"
#include "structs/player.h"

/* A looping sound: only its sound handle here. */
struct CLoopingSoundData {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 m_nHandle;
};

void func_0027588C(s32);                            /* extern */
void func_00275D34(s32, struct CLoopingSoundData *); /* extern */
s32 func_00275F7C();                                /* extern */

/* Stops the player's weapon firing looping sound. */
void func_00253F40(CPlayer *arg0) {
    struct CLoopingSoundData *temp_v0;

    temp_v0 = arg0->m_pLoopingSound_WeaponFiring;
    if (temp_v0 != NULL) {
        func_0027588C(temp_v0->m_nHandle);
    }
    func_00275D34(func_00275F7C(), arg0->m_pLoopingSound_WeaponFiring);
    arg0->m_pLoopingSound_WeaponFiring = NULL;
}
