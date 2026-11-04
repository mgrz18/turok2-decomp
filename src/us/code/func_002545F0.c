#include "common.h"
#include "structs/player.h"

/* The region info func_00268A2C returns; only 0x2C here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 unk2C;
} RegionInfo;

RegionInfo *func_00268A2C(s32);                     /* extern */

void func_002545F0(CPlayer *arg0, s32 arg1) {
    f32 temp_f1;
    RegionInfo *temp_v0;

    temp_v0 = func_00268A2C(arg1);
    if (temp_v0 != NULL) {
        arg0->m_vWeaponFirePos.y += (temp_v0->unk2C - arg0->m_vWeaponFirePos.y) * 0.2f;
    }
}
