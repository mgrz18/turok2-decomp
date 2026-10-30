#include "common.h"

typedef struct Actor Actor;

struct Actor {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x10C];
    /* 0x24C */ s32 unk24C;
};

typedef struct {
    /* 0x00 */ void *proc;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 time;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} Timer;

typedef struct {
    /* 0x000 */ Actor *leader;
    /* 0x004 */ s32 countA;
    /* 0x008 */ Actor *groupA[4];
    /* 0x018 */ s32 countB;
    /* 0x01C */ Actor *groupB[4];
    /* 0x02C */ s32 countC;
    /* 0x030 */ Actor *groupC[(0x4F8 - 0x30) / 4];
    /* 0x4F8 */ s32 *script;
    /* 0x4FC */ s32 command;
    /* 0x500 */ s32 unk500;
    /* 0x504 */ s32 unk504;
    /* 0x508 */ s32 unk508;
    /* 0x50C */ s32 count454;
    /* 0x510 */ s32 count455;
    /* 0x514 */ s32 count456;
    /* 0x518 */ Timer timers[3];
} Manager;

typedef struct {
    /* 0x0 */ void (*run)(Manager *m);
    /* 0x4 */ s32 unk4;
} Command;

extern Command D_0043240C[];

extern u8 D_00432494[];
extern u8 D_800F7078[];
extern void func_00225C84(void *);
extern s32 func_00243414(Actor *actor, void *ai, s32 state);

static __inline__ void StartTimer(Manager *m, void *proc, f32 seconds) {
    Timer *t;

    m->timers[0].proc = proc;
    t = &m->timers[0];
    t->unk4 = 0;
    t->unkC = 0;
    t->unk10 = 0;
    t->unk14 = 0;
    t->time = seconds * 15.0f;
}

void func_0040E59C(Manager *m) {
    s32 i;

    for (i = 0; i < m->countA; i++) {
        func_00243414(m->groupA[i], m->groupA[i]->ai, 0);
    }
    StartTimer(m, D_00432494, ((f32 *) m->script)[1]);
    func_00225C84(D_800F7078);
}
