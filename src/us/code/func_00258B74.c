#include "common.h"
#include "structs/game_object.h"
#include "globals/player.h"
#include "protos/ai.h"

s32 CPlayer__GetWeaponAmmo(s32, s32);
s32 CPlayer__GetNextUsableWeapon(s32);
s32 WeaponActor_CheckFire(s32, s32);


/* The AI parent of the actor (m_AI.m_pParent), seen as a larger object. */
typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 unk996;
    /* 0x998 */ u8 pad998[0xA08 - 0x998];
    /* 0xA08 */ s16 unkA08;   /* row in gPlayerStateTable */
    /* 0xA0A */ u8 padA0A[0xB14 - 0xA0A];
    /* 0xB14 */ s16 unkB14;
} Unk00258B74;

void WeaponMode_Ready_Update(CGameObjectInstance *arg0, s32 arg1) {
    s32 temp_s2;
    Unk00258B74 *temp_s0;

    temp_s0 = (Unk00258B74 *)arg0->m_AI.m_pParent;
    temp_s2 = gPlayerStateTable[temp_s0->unkA08].pose;
    if (CPlayer__GetWeaponAmmo((s32) temp_s0, (s32) temp_s0->unk996) == 0) {
        temp_s0->unkB14 = CPlayer__GetNextUsableWeapon((s32) temp_s0);
    }
    if ((WeaponActor_CheckFire((s32) arg0, arg1) == 0) && !(arg0->m_dwFlags & 0x400)) {
        CAI__SetMode((s32) arg0, arg1, (s32) temp_s2);
    }
}
