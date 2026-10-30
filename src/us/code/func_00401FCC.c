#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00401FCC;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00401FCC pos;
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

typedef struct {
    /* 0x000 */ Entity00401FCC *actor;
    /* 0x004 */ u8 pad0[0x218 - 0x4];
    /* 0x218 */ s32 total;
    /* 0x21C */ u8 pad1[0x224 - 0x21C];
    /* 0x224 */ Member00401FCC *members[8];
    /* 0x244 */ s32 count;
} State00401FCC;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 maxActive;
} Config00401FCC;

extern u8 D_80119870[];
extern Config00401FCC *D_8012F5B4;
Entity00401FCC *func_002532EC(void *game, Vec3_00401FCC pos);
f32 func_002465F0(Member00401FCC *member, Vec3_00401FCC pos);
f32 func_002119FC(f32 lo, f32 hi);
s32 func_00243414(Member00401FCC *member, void *ai, s32 event);
void func_00226F90(void *base, s32 count, s32 size, s32 (*cmp)(), void (*swap)());
s32 func_004035CC();
void func_004035B8();

void func_00401FCC(State00401FCC *s, s32 *active) {
    Config00401FCC *cfg = D_8012F5B4;
    Entity00401FCC *player;
    Vec3_00401FCC pos;
    Member00401FCC *member;
    s32 i;

    player = func_002532EC(D_80119870, s->actor->pos);
    if (player != NULL) {
        pos = player->pos;
    } else {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
    }
    for (i = 0; i < s->count; i++) {
        member = s->members[i];
        member->distance = func_002465F0(member, pos);
        member->unk250 = 1;
        if ((member->u174.word & 0xFFFF0000) == 0x10000) {
            member->delay = func_002119FC(0.75f, 1.25f);
        }
    }
    func_00226F90(s->members, s->count, 4, func_004035CC, func_004035B8);
    for (i = 0; i < s->count && s->total != 0 && *active < cfg->maxActive; i++) {
        member = s->members[i];
        if (member->u174.state == 0 && func_00243414(member, member->ai, 1)) {
            member->delay = func_002119FC(1.5f, 2.0f);
            s->total--;
            (*active)++;
        }
    }
}
