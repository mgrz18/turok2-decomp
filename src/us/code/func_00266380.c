#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

#include "structs/intelligence.h"
#include "structs/game.h"



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

extern CPlayer *CPlayerPool__FindTagged(CPlayerList *list);
extern void CAI__SetMode(CGameObjectInstance *actor, ActionState *state, s32 mode);

void Action_GoUpdate(CGameObjectInstance *actor, ActionState *state) {
    CDoorIntelligence *info = &actor->header.m_pIntelligence->m_Door;
    CGame *game;
    CPlayer *player;
    s32 mode;

    if (actor->m_nTypeFlag == 0x64E && state->unk36 != state->unk34) {
        game = &D_80119870;
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
