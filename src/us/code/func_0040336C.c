#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"



typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Obj0040336C;

typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ s32 unkF8;
} Item0040336C;

void CPlayerPool__SetInvincibility(CPlayerPool *list, f32 value);
s32 Cinema_IsIdle(void);
void CPlayer__Warp(CPlayer *player, s32 a1, s32 a2, s32 a3);

void MantidQueen_UpdateDeath(Obj0040336C *obj, Item0040336C *item) {
    CGame *game = &gEngineApp_m_Game;
    CPlayer *player;

    CPlayerPool__SetInvincibility(&game->m_PlayerList, 900.0f);
    item->unkF8 = 0;
    obj->flags &= ~0x2000;
    if (Cinema_IsIdle()) {
        player = CPlayerPool__GetFirstActive(&game->m_PlayerList);
        if (player != NULL) {
            GameSettings *sub = &game->m_Settings;

            player->m_Data.m_bQueenBossComplete = 1;
            sub->unlockedCheats |= 0x6000;
            CPlayer__Warp(player, 0x176F, 2, 0);
        }
    }
}
