#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/game.h"

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
s32 CAI__SetMode(Member00401FCC *member, void *ai, s32 event);
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), void (*swap)());
s32 MantidQueen_SortCompare();
void MantidQueen_SortSwap();

void MantidQueen_SendMinions(WaveBossState *s, s32 *active) {
    WaveBossStep *cfg = gMantidQueen_step;
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
