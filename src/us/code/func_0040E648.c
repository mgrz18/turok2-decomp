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

void func_0040E648(Manager *m) {
    s32 *script;
    s32 cmd;
    s32 i;

    if (m->count454 == 0) {
        script = m->script;
        m->script = script + 3;
        cmd = script[3];
        m->timers[0].proc = NULL;
        m->timers[1].proc = NULL;
        m->timers[2].proc = NULL;
        for (i = 0; i < m->countA; i++) {
            m->groupA[i]->unk24C = 0;
        }
        for (i = 0; i < m->countB; i++) {
            m->groupB[i]->unk24C = 0;
        }
        for (i = 0; i < m->countC; i++) {
            m->groupC[i]->unk24C = 0;
        }
        m->command = cmd;
        if (D_0043240C[cmd].run != NULL) {
            D_0043240C[cmd].run(m);
        }
    }
}
