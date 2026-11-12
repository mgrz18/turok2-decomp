#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"



#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the views at the host offsets (tools/port/hostview.py; round 10 y2): the actor's m_dwFlags and
 * the CAI's m_pfHurt (N64 0xF8), cleared as a whole pointer (an s32 store left its high half) */
#define T2_HOFF_Obj0040336C(m) __builtin_offsetof(CGameObjectInstance, m)
#define T2_HOFF_Item0040336C(m) __builtin_offsetof(CAI, m)
typedef struct {
    union {
        struct { u8 _hp_flags[T2_HOFF_Obj0040336C(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
    };
} Obj0040336C;

typedef struct {
    union {
        struct { u8 _hp_unkF8[T2_HOFF_Item0040336C(m_pfHurt)]; void *unkF8; }; /* N64 0xF8 */
    };
} Item0040336C;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Obj0040336C;

typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ s32 unkF8;
} Item0040336C;
#endif

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
