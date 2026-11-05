#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/player.h"

/* Boss fight AI tick: runs the fight phase (timer and helpers, then the
 * random stun of a live helper), and once the boss has waited long enough
 * outside the fight starts the level's boss music. */

typedef struct {
    /* 0x00 */ void *obj;
    /* 0x04 */ void *def;
    /* 0x08 */ s32 state;
    /* 0x0C */ f32 timer;
} BossFight;

extern BossFight D_8012FB50;
extern CListedInstance *D_8010FEB4;
extern f32 D_800B6D28;
extern s32 D_80110048;

extern void func_004089E0();
extern void func_004082A0(BossFight *fight);
extern f32 func_0021149C(f32 range);
extern void func_00246690(CListedInstance *actor, s32 sound, CVector3 pos, s32 arg);
extern void func_00245828(void *out, s32 a, s32 b, f32 c, s32 d, s32 e);
extern void func_0025B2D4(CListedInstance *actor, void *ai, void *anim);
extern CPlayer *func_002532A8(void *players);
extern void func_00248BA8(CPlayer *player, s32 track, s32 a, s32 b);

static __inline__ s32 helperAlive(void) {
    CListedInstance *actor;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}

static __inline__ void stunHelper(void) {
    u8 anim[0x18];
    CListedInstance *actor;

    if (D_8012FB50.timer > 0.0f) {
        D_8012FB50.timer -= D_800B6D28;
    } else if (!(func_0021149C(100.0f) > 25.0f)) {
        for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
            if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
                func_00246690(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
                func_00245828(anim, 0, 0xFF00, 51.199997f, 0x100000, 0);
                func_0025B2D4(actor, &actor->m_Instance.m_AI, anim);
                break;
            }
        }
    }
}

void func_004085A0(void *obj, CAI *ai) {
    BossFight *fight = &D_8012FB50;
    CPlayer *player;
    s32 track;

    if (ai->m_pfDraw != func_004089E0) {
        ai->m_pfDraw = func_004089E0;
    }
    switch (fight->state) {
    case 0:
        func_004082A0(fight);
        break;
    case 1:
        stunHelper();
        break;
    case 2:
        helperAlive();
        break;
    }
    if (fight->state == 0) {
        ai->m_ModeTime = 0.0f;
        return;
    }
    if (fight->state == 1) {
        if (helperAlive()) {
            ai->m_ModeTime = 0.0f;
            return;
        }
        if (ai->m_ModeTime < 90.0f) {
            return;
        }
    } else if (ai->m_ModeTime < 75.0f) {
        return;
    }
    player = func_002532A8(&D_80119870.m_PlayerList);
    if (player == NULL) {
        return;
    }
    switch (D_80110048) {
    case 0x1BBC:
        track = 0x7CF;
        break;
    case 0x1C20:
        track = 0xBB7;
        break;
    case 0x1C84:
        track = 0xF9F;
        break;
    case 0x1CE8:
        track = 0x1CE9;
        if (player->m_Data.m_bBlindBossComplete) {
            track = 0x1387;
        }
        break;
    case 0x1D4C:
        track = 0x1D4D;
        if (player->m_Data.m_bQueenBossComplete) {
            track = 0x176F;
        }
        break;
    default:
        track = D_80110048;
        break;
    }
    player->m_InvincibilityTime = 0.0f;
    func_00248BA8(player, track, 2, 0);
}
