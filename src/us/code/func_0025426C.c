#include "common.h"
#include "structs/player.h"

s32 func_002241D8(s32 *, void *, u16, s32); /* extern */
/* Per-weapon info rows (pointers indexed by weapon). */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u16 unk4;
} WeaponInfo;
extern WeaponInfo *D_800AF618[];

struct CAIModeInfo {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 (*func)();
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 unk10;
};
extern s32 D_800F7078;

void func_0025426C(CPlayer *arg0) {
    func_002241D8(&D_800F7078, &arg0->m_WeaponActor, D_800AF618[arg0->m_Data.m_CurrentWeapon]->unk4, arg0->m_WeaponActor.m_AI.m_pModeTable->unk10);
}
