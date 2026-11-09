#include "common.h"
#include "structs/player.h"
#include "structs/region.h"
#include "structs/game.h"
#include "structs/collision.h"
#include "structs/particle.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/player.h"

typedef struct TeamGame {
    /* 0x00 */ s32 active;
} TeamGame;

extern s32 D_8011ACE0;      /* team 1 score */

s32 Cinema_PathPlaying(void);
s32 Cinema_IsActive(void);
void CPlayer__Hurt(CPlayer *player, CGameObjectInstance *actor, s32 damage, s32 arg3, s32 arg4);
void CPlayer__ShowKillMessages(CPlayer *victim, CPlayer *killer, DamageEvent *hit);

static __inline__ s32 inSpecialRegion(CPlayer *player) {
    CRegionInfo *info;

    if (player->m_CharacterActor.header.m_pCurrentRegion != NULL) {
        info = CScene__GetRegionInfo(&gScene, player->m_CharacterActor.header.m_pCurrentRegion);
        if (info != NULL) {
            if (info->m_wFlags52 & 0x40) {
                return 1;
            }
        }
    }
    return 0;
}

/* A player's actor took a lethal hit in multiplayer: works out the killer and
   updates the kill, death, tag and team counts. */
void Player_OnMultiplayerKill(CGameObjectInstance *actor, s32 arg1, DamageEvent *hit) {
    CPlayer *player;
    CPlayer *killer = NULL;
    s32 arg4 = -1;
    CRegionInfo *info;
    s32 special;
    CGameObjectInstance *source;
    CGameObjectInstance *owner;
    CPlayerOptions *options;
    s32 killerIndex;
    s32 victimIndex;
    s32 *score;
    s32 *teamScore;

    if (Cinema_PathPlaying() != 0) {
        return;
    }
    if (gEngineApp_m_Settings.cheatFlags & 1) {
        return;
    }
    if (Cinema_IsActive() != 0) {
        return;
    }
    player = (CPlayer *) actor->m_AI.m_pParent;
    special = inSpecialRegion(player);
    if (special) {
        arg4 = 20;
    }
    source = hit->source;
    if (source != NULL && source->header.m_Type == 2 && ((CParticle *) source)->m_pOwner == (CGameObjectInstance *) player) {
        hit->damage = hit->damage * 0.25f;
    }
    CPlayer__Hurt(player, actor, hit->damage, hit->flags, arg4);

    source = hit->source;
    if (source != NULL) {
        switch (source->header.m_Type) {
            case 2:
                owner = ((CParticle *) source)->m_pOwner;
                if (owner != NULL && owner->header.m_Type == 1 && (owner->m_dwFlags & 0x300000)) {
                    killer = (CPlayer *) owner;
                }
                break;
            case 1:
                if (source->m_dwFlags & 0x300000) {
                    killer = (CPlayer *) source->m_AI.m_pParent;
                }
                break;
        }
    } else {
        killer = player;
    }

    if (killer == NULL) {
        return;
    }
    if (player->m_Data.m_Health != 0) {
        return;
    }
    if (player->m_pOptions->m_bDead != 0) {
        return;
    }
    player->m_pOptions->m_bDead = 1;
    killerIndex = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, killer);
    options = player->m_pOptions;
    if (options->m_TagMode == 1) {
        options->m_TaggedBy[killerIndex]++;
    } else {
        options->m_KilledBy[killerIndex]++;
    }
    victimIndex = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    if (player->m_pOptions->m_TagMode == 1) {
        killer->m_pOptions->m_Tagged[victimIndex]++;
    } else {
        killer->m_pOptions->m_Killed[victimIndex]++;
    }
    CPlayer__ShowKillMessages(player, killer, hit);

    if (killerIndex == victimIndex) {
        player->m_pOptions->m_Suicides++;
        if (((TeamGame *)&gEngineApp_m_Settings_match_teams)->active != 0) {
            switch (player->m_nDeathmatchTeamSelected) {
                case 0:
                    teamScore = &gEngineApp_m_Settings.match.teamScore;
                    break;
                case 1:
                    teamScore = &D_8011ACE0;
                    break;
                default:
                    return;
            }
            (*teamScore)--;
        }
    } else {
        player->m_pOptions->m_Deaths++;
        if (player->m_pOptions->m_TagMode == 1) {
            killer->m_pOptions->m_Tags++;
            if (((TeamGame *)&gEngineApp_m_Settings_match_teams)->active != 0) {
                switch (killer->m_nDeathmatchTeamSelected) {
                    case 0:
                        score = &gEngineApp_m_Settings.match.teamScore;
                        break;
                    case 1:
                        score = &D_8011ACE0;
                        break;
                    default:
                        return;
                }
                if (player->m_nDeathmatchTeamSelected != killer->m_nDeathmatchTeamSelected) {
                    (*score)++;
                }
            }
        } else {
            killer->m_pOptions->m_Kills++;
            if (((TeamGame *)&gEngineApp_m_Settings_match_teams)->active != 0) {
                switch (killer->m_nDeathmatchTeamSelected) {
                    case 0:
                        score = &gEngineApp_m_Settings.match.teamScore;
                        break;
                    case 1:
                        score = &D_8011ACE0;
                        break;
                    default:
                        return;
                }
                if (player->m_nDeathmatchTeamSelected != killer->m_nDeathmatchTeamSelected) {
                    (*score)++;
                }
            }
        }
    }
}
