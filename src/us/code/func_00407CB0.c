/* (&D_80119870.m_Settings)->unlockedCheats instead of D_80119870.m_Settings.unlockedCheats gives the ROM's register choice (found
 * with the permuter). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "structs/player.h"

/* The object this callback gets (only +0xF8 is used). */
typedef struct {
    /* 0x00 */ u8 pad00[0xF8];
    /* 0xF8 */ void *held;
} Holder;

extern void func_002535E4(CPlayerList *list, f32 arg1);
extern CPlayer *func_002532A8(CPlayerList *list);
extern void func_00412438(s32 id);


void func_00407CB0(s32 arg0, Holder *obj) {
    CPlayer *player;
    GameSettings *flags;

    obj->held = NULL;
    func_002535E4(&D_80119870.m_PlayerList, 900.0f);
    player = func_002532A8(&D_80119870.m_PlayerList);
    player->m_Data.m_bPrimagenBossComplete = 1;
    if (player->m_Data.m_bLevel1TotemFlags && player->m_Data.m_bLevel2TotemFlags && player->m_Data.m_bLevel3TotemFlags
        && player->m_Data.m_bLevel4TotemFlags && player->m_Data.m_bLevel5TotemFlags) {
        func_00412438(0x172);
        (&D_80119870.m_Settings)->unlockedCheats |= 0x7FF0019;
    } else {
        func_00412438(0x173);
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 2;
    }
}
