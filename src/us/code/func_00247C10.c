#include "common.h"
#include "structs/player.h"
#include "structs/region.h"
#include "structs/game.h"
#include "structs/collision.h"

typedef struct Actor {
    /* 0x000 */ u8 kind;                /* 1 = actor, 2 = particle */
    /* 0x001 */ u8 pad1[0xD4 - 0x1];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x10C - 0xD8];
    /* 0x10C */ struct Actor *particleOwner;
    /* 0x110 */ u8 pad110[0x1A8 - 0x110];
    /* 0x1A8 */ struct CPlayer *owner;
} Actor;

extern u8 D_800F7078[];
typedef struct TeamGame {
    /* 0x00 */ s32 active;
} TeamGame;

extern TeamGame D_8011ACD4;
extern s32 D_8011ACDC;      /* team 0 score */
extern s32 D_8011ACE0;      /* team 1 score */

s32 Cinema_PathPlaying(void);
s32 func_00284174(void);
CRegionInfo *func_0022425C(void *world, void *region);
void func_00249358(CPlayer *player, Actor *actor, s32 damage, s32 arg3, s32 arg4);
s32 func_0025340C(void *players, CPlayer *player);
void func_002478C8(CPlayer *victim, CPlayer *killer, DamageEvent *hit);

static __inline__ s32 inSpecialRegion(CPlayer *player) {
    CRegionInfo *info;

    if (player->m_CharacterActor.header.m_pCurrentRegion != NULL) {
        info = func_0022425C(D_800F7078, player->m_CharacterActor.header.m_pCurrentRegion);
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
void func_00247C10(Actor *actor, s32 arg1, DamageEvent *hit) {
    CPlayer *player;
    CPlayer *killer = NULL;
    s32 arg4 = -1;
    CRegionInfo *info;
    s32 special;
    Actor *source;
    Actor *owner;
    CPlayerOptions *options;
    s32 killerIndex;
    s32 victimIndex;
    s32 *score;
    s32 *teamScore;

    if (Cinema_PathPlaying() != 0) {
        return;
    }
    if (D_8011AAD8.cheatFlags & 1) {
        return;
    }
    if (func_00284174() != 0) {
        return;
    }
    player = actor->owner;
    special = inSpecialRegion(player);
    if (special) {
        arg4 = 20;
    }
    source = hit->source;
    if (source != NULL && source->kind == 2 && source->particleOwner == (Actor *) player) {
        hit->damage = hit->damage * 0.25f;
    }
    func_00249358(player, actor, hit->damage, hit->flags, arg4);

    source = hit->source;
    if (source != NULL) {
        switch (source->kind) {
            case 2:
                owner = source->particleOwner;
                if (owner != NULL && owner->kind == 1 && (owner->flags & 0x300000)) {
                    killer = (CPlayer *) owner;
                }
                break;
            case 1:
                if (source->flags & 0x300000) {
                    killer = source->owner;
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
    killerIndex = func_0025340C(&D_80119870.m_PlayerList, killer);
    options = player->m_pOptions;
    if (options->m_TagMode == 1) {
        options->m_TaggedBy[killerIndex]++;
    } else {
        options->m_KilledBy[killerIndex]++;
    }
    victimIndex = func_0025340C(&D_80119870.m_PlayerList, player);
    if (player->m_pOptions->m_TagMode == 1) {
        killer->m_pOptions->m_Tagged[victimIndex]++;
    } else {
        killer->m_pOptions->m_Killed[victimIndex]++;
    }
    func_002478C8(player, killer, hit);

    if (killerIndex == victimIndex) {
        player->m_pOptions->m_Suicides++;
        if (D_8011ACD4.active != 0) {
            switch (player->m_nDeathmatchTeamSelected) {
                case 0:
                    teamScore = &D_8011ACDC;
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
            if (D_8011ACD4.active != 0) {
                switch (killer->m_nDeathmatchTeamSelected) {
                    case 0:
                        score = &D_8011ACDC;
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
            if (D_8011ACD4.active != 0) {
                switch (killer->m_nDeathmatchTeamSelected) {
                    case 0:
                        score = &D_8011ACDC;
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
