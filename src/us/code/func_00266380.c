#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

#include "structs/intelligence.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/ai.h"



typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 unk34;
    /* 0x035 */ u8 pad35;
    /* 0x036 */ s8 unk36;
    /* 0x037 */ u8 pad37[0x90];
    /* 0x0C7 */ s8 active;
    /* 0x0C8 */ u8 padC8[0x48];
    /* 0x110 */ s32 done;
    /* 0x114 */ s32 tagCleared;
} ActionState;

extern CPlayer *CPlayerPool__FindTagged(CPlayerPool *list);

#ifdef TARGET_PORT
/* The port (round 9 x1): the action's CAI by member. The N64 view's 0x34/0x36 are m_Mode and
 * m_ModeBefore, 0xC7 m_AnimRepeatedCount (the go animation played through), 0x110/0x114
 * m_VarBuffer[1]/[2] (Action_Setup and Action_IdleEnter clear them); on the host those offsets
 * fall elsewhere in CAI (its pointers are wider), so a used switch never left mode 1. */
void Action_GoUpdate(CGameObjectInstance *actor, ActionState *state_) {
    CAI *state = (CAI *) state_;
    CDoorIntelligence *info = &actor->header.m_pIntelligence->m_Door;
    CGame *game;
    CPlayer *player;
    s32 mode;

    if (actor->m_nTypeFlag == 0x64E && state->m_ModeBefore != state->m_Mode) {
        game = &gEngineApp_m_Game;
        player = CPlayerPool__FindTagged(&game->m_PlayerList);
        if (player != NULL && state->m_VarS32[2] == 0) {
            mode = player->m_pOptions->m_TagMode;
            if (mode == 1) {
                player->m_pOptions->m_TagMode = 0;
                player->m_pOptions->m_LastTagMode = mode;
                game->m_Settings.match.tagPhase = mode;
                state->m_VarS32[2] = mode;
            }
        }
    }
    if (state->m_AnimRepeatedCount != 0) {
        if (info->m_dwFlags & 2) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (state->m_VarS32[1] != 0) {
            CAI__SetMode(actor, state, 0);
        }
        if (actor->m_nTypeFlag == 0x64C) {
            CAI__SetMode(actor, state, 2);
        }
    }
}
#else
void Action_GoUpdate(CGameObjectInstance *actor, ActionState *state) {
    CDoorIntelligence *info = &actor->header.m_pIntelligence->m_Door;
    CGame *game;
    CPlayer *player;
    s32 mode;

    if (actor->m_nTypeFlag == 0x64E && state->unk36 != state->unk34) {
        game = &gEngineApp_m_Game;
        player = CPlayerPool__FindTagged(&game->m_PlayerList);
        if (player != NULL && state->tagCleared == 0) {
            mode = player->m_pOptions->m_TagMode;
            if (mode == 1) {
                player->m_pOptions->m_TagMode = 0;
                player->m_pOptions->m_LastTagMode = mode;
                game->m_Settings.match.tagPhase = mode;
                state->tagCleared = mode;
            }
        }
    }
    if (state->active != 0) {
        if (info->m_dwFlags & 2) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (state->done != 0) {
            CAI__SetMode(actor, state, 0);
        }
        if (actor->m_nTypeFlag == 0x64C) {
            CAI__SetMode(actor, state, 2);
        }
    }
}
#endif
