#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} LoopSound;

s32 func_0027588C(s32);                         /* extern */
s32 func_00275D34(s32, void *);                 /* extern */
s32 func_00275F7C();                                /* extern */

void func_00253E68(CPlayer *arg0) {
    LoopSound *temp_v0;

    temp_v0 = arg0->m_pLoopingSound_WeaponBackground;
    if (temp_v0 != NULL) {
        func_0027588C(temp_v0->id);
    }
    func_00275D34(func_00275F7C(), arg0->m_pLoopingSound_WeaponBackground);
    arg0->m_pLoopingSound_WeaponBackground = NULL;
}
