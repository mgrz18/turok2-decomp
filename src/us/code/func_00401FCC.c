#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/game.h"
#include "globals/boss.h"
#include "protos/ai.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CVector3 pos;
} Entity00401FCC;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x174 - 0x140];
    /* 0x174 */ union {
        s32 word;
        s8 state;
    } u174;
    /* 0x178 */ u8 pad1[0x1FC - 0x178];
    /* 0x1FC */ f32 delay;
    /* 0x200 */ u8 pad2[0x24C - 0x200];
    /* 0x24C */ f32 distance;
    /* 0x250 */ s32 unk250;
} Member00401FCC;

Entity00401FCC *CPlayerPool__FindNearest(void *game, CVector3 pos);
f32 AI_DistSqToPointAtHeight(Member00401FCC *member, CVector3 pos);
f32 Random_Blend(f32 lo, f32 hi);
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), void (*swap)());
s32 MantidQueen_SortCompare();
void MantidQueen_SortSwap();

#ifdef TARGET_PORT
#include "structs/player.h"
/* Host body (round 9 x2): the minions by member. The N64 view (Member00401FCC) put the CAI at
 * 0x140, the mode word at 0x174 (tested big-endian: mode 0, last mode 1), the anim speed at 0x1FC
 * and the sort key and flag at 0x24C/0x250 (m_AI.m_VarF32[0], m_VarS32[1]); and Sort_Quick
 * sorted the member pointers as 4-byte words. No minion was ever sent in: the wave's toSpawn
 * stayed 3 and the Queen's spots stayed shut. */
void MantidQueen_SendMinions(WaveBossState *s, s32 *active) {
    WaveBossStep *cfg = gMantidQueen.step;
    struct CPlayer *player;
    CVector3 pos;
    CGameObjectInstance *member;
    s32 i;

    player = (struct CPlayer *) CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, s->actor->header.m_vPos);
    if (player != NULL) {
        pos = player->m_CharacterActor.header.m_vPos;
    } else {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
    }
    for (i = 0; i < s->memberCount; i++) {
        member = s->members[i];
        member->m_AI.m_VarF32[0] = AI_DistSqToPointAtHeight((Member00401FCC *) member, pos);
        member->m_AI.m_VarS32[1] = 1;
        if (member->m_AI.m_Mode == 0 && member->m_AI.m_ModeLastFrame == 1) {
            member->m_AI.m_AnimSpeed = Random_Blend(0.75f, 1.25f);
        }
    }
    Sort_Quick(s->members, s->memberCount, sizeof(s->members[0]), MantidQueen_SortCompare, MantidQueen_SortSwap);
    for (i = 0; i < s->memberCount && s->toSpawn != 0 && *active < cfg->maxActive; i++) {
        member = s->members[i];
        if (member->m_AI.m_Mode == 0 && CAI__SetMode(member, &member->m_AI, 1)) {
            member->m_AI.m_AnimSpeed = Random_Blend(1.5f, 2.0f);
            s->toSpawn--;
            (*active)++;
        }
    }
}
#else
void MantidQueen_SendMinions(WaveBossState *s, s32 *active) {
    WaveBossStep *cfg = gMantidQueen.step;
    Entity00401FCC *player;
    CVector3 pos;
    Member00401FCC *member;
    s32 i;

    player = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, s->actor->header.m_vPos);
    if (player != NULL) {
        pos = player->pos;
    } else {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
    }
    for (i = 0; i < s->memberCount; i++) {
        member = (Member00401FCC *) s->members[i];
        member->distance = AI_DistSqToPointAtHeight(member, pos);
        member->unk250 = 1;
        if ((member->u174.word & 0xFFFF0000) == 0x10000) {
            member->delay = Random_Blend(0.75f, 1.25f);
        }
    }
    Sort_Quick(s->members, s->memberCount, 4, MantidQueen_SortCompare, MantidQueen_SortSwap);
    for (i = 0; i < s->memberCount && s->toSpawn != 0 && *active < cfg->maxActive; i++) {
        member = (Member00401FCC *) s->members[i];
        if (member->u174.state == 0 && CAI__SetMode(member, member->ai, 1)) {
            member->delay = Random_Blend(1.5f, 2.0f);
            s->toSpawn--;
            (*active)++;
        }
    }
}
#endif
