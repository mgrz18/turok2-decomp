#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* The controller of a boss fight (Owner00403430 in MantidQueen_UpdateBossAnim): it holds the boss at 0x220. */
#ifdef TARGET_PORT
#include "structs/boss.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_boss[__builtin_offsetof(WaveBossState, boss)]; CGameObjectInstance * boss; };
    };
} BossOwner;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x220];
    /* 0x220 */ CGameObjectInstance *boss;
} BossOwner;
#endif


void MantidQueen_SetBossAnim(BossOwner *arg0, s32 arg1) {
    CGameObjectInstance *temp_a0;

    temp_a0 = arg0->boss;
    AI_PlayAnim((sptr) temp_a0, &temp_a0->m_AI, arg1);
}
