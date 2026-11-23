/* (&gEngineApp_m_Game.m_Settings)->unlockedCheats instead of gEngineApp_m_Game.m_Settings.unlockedCheats gives the ROM's register choice (found
 * with the permuter). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "structs/player.h"
#include "globals/game.h"
#include "protos/player.h"

/* The object this callback gets (only +0xF8 is used). */
#ifdef TARGET_PORT
/* the CAI's m_pfHurt at its host offset (tools/port/hostview.py; round 10 y2): the N64 0xF8
 * cleared other bytes, the hurt callback stayed */
typedef struct {
    union {
        struct { u8 _hp_held[__builtin_offsetof(CAI, m_pfHurt)]; void *held; }; /* N64 0xF8 */
    };
} Holder;
#else
typedef struct {
    /* 0x00 */ u8 pad00[0xF8];
    /* 0xF8 */ void *held;
} Holder;
#endif

extern void CPlayerPool__SetInvincibility(CPlayerPool *list, f32 arg1);
extern void Cinema_RequestById(s32 id);


void Primagen_EnterDeath(sptr arg0, Holder *obj) {
    CPlayer *player;
    GameSettings *flags;

    obj->held = NULL;
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    player->m_Data.m_bPrimagenBossComplete = 1;
    if (player->m_Data.m_bLevel1TotemFlags && player->m_Data.m_bLevel2TotemFlags && player->m_Data.m_bLevel3TotemFlags
        && player->m_Data.m_bLevel4TotemFlags && player->m_Data.m_bLevel5TotemFlags) {
        Cinema_RequestById(0x172);
        (&gEngineApp_m_Game.m_Settings)->unlockedCheats |= 0x7FF0019;
    } else {
        Cinema_RequestById(0x173);
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 2;
    }
}
