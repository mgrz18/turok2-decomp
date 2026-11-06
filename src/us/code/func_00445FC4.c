/* Second-module copy of BlindOne_PhaseWaitPlayerUpdate (in src/us/code/BlindOne_UpdateMover.c). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Script command: waits until the current player is within 716.8 units of the origin, then steps the
 * script to the next command. */


extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
extern f32 func_00298470(f32);

/* Advances the script by one command, stops the movers and runs the new command. */
static __inline__ void NextCommand(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    script = m->script;
    m->script = script + 1;
    cmd = script[1].id;
    m->movers[0].seq = NULL;
    m->movers[1].seq = NULL;
    m->movers[2].seq = NULL;
    for (i = 0; i < m->count1; i++) {
        m->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count2; i++) {
        m->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count3; i++) {
        m->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    m->phase = cmd;
    if (sBlindOnePhases_2[cmd].init != NULL) {
        sBlindOnePhases_2[cmd].init(m);
    }
}

void BlindOne_PhaseWaitPlayerUpdate_2(BlindOneState *m) {
    CPlayer *player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;

    if (player != NULL) {
        if (func_00298470(player->m_CharacterActor.header.m_vPos.x * player->m_CharacterActor.header.m_vPos.x + player->m_CharacterActor.header.m_vPos.y * player->m_CharacterActor.header.m_vPos.y +
                          player->m_CharacterActor.header.m_vPos.z * player->m_CharacterActor.header.m_vPos.z) < 716.8f) {
            NextCommand(m);
        }
    }
}
