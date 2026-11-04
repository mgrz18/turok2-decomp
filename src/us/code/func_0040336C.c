#include "common.h"
#include "structs/player.h"
#include "structs/game.h"



typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Obj0040336C;

typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ s32 unkF8;
} Item0040336C;

void func_002535E4(CPlayerList *list, f32 value);
s32 func_00284204(void);
CPlayer *func_002532A8(CPlayerList *list);
void func_00248BA8(CPlayer *player, s32 a1, s32 a2, s32 a3);

void func_0040336C(Obj0040336C *obj, Item0040336C *item) {
    CGame *game = &D_80119870;
    CPlayer *player;

    func_002535E4(&game->m_PlayerList, 900.0f);
    item->unkF8 = 0;
    obj->flags &= ~0x2000;
    if (func_00284204()) {
        player = func_002532A8(&game->m_PlayerList);
        if (player != NULL) {
            GameSettings *sub = &game->m_Settings;

            player->m_Data.m_bQueenBossComplete = 1;
            sub->unlockedCheats |= 0x6000;
            func_00248BA8(player, 0x176F, 2, 0);
        }
    }
}
