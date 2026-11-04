/* Second-module copy of func_0040CFC4 (in src/us/code/func_0040C850.c). */
#include "common.h"
#include "structs/player.h"

/* Script command: waits until the current player is within 716.8 units of the origin, then steps the
 * script to the next command. */

typedef struct {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ s32 unk24C;
} Actor;

typedef struct {
    /* 0x00 */ void *proc;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
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
    /* 0x500 */ u8 pad500[0x518 - 0x500];
    /* 0x518 */ Timer timers[3];
} Manager;

typedef struct {
    /* 0x0 */ void (*run)(Manager *m);
    /* 0x4 */ s32 unk4;
} Command;

extern Command D_0043232C[];
extern CPlayer *D_80119890;
extern f32 func_00298470(f32);

/* Advances the script by one command, stops the timers and runs the new command. */
static __inline__ void NextCommand(Manager *m) {
    s32 *script;
    s32 cmd;
    s32 i;

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
    if (D_0043232C[cmd].run != NULL) {
        D_0043232C[cmd].run(m);
    }
}

void func_00445FC4(Manager *m) {
    CPlayer *player = D_80119890;

    if (player != NULL) {
        if (func_00298470(player->m_CharacterActor.header.m_vPos.x * player->m_CharacterActor.header.m_vPos.x + player->m_CharacterActor.header.m_vPos.y * player->m_CharacterActor.header.m_vPos.y +
                          player->m_CharacterActor.header.m_vPos.z * player->m_CharacterActor.header.m_vPos.z) < 716.8f) {
            NextCommand(m);
        }
    }
}
