#include "common.h"

/* Multiplayer stats kept per player (player->m_pOptions). */
typedef struct PlayerOptions {
    /* 0x00 */ u16 suicides;
    /* 0x02 */ u16 deaths;
    /* 0x04 */ u16 kills;
    /* 0x06 */ u16 tags;
    /* 0x08 */ u16 taggedBy[4];
    /* 0x10 */ u16 tagged[4];
    /* 0x18 */ u16 killedBy[4];
    /* 0x20 */ u16 killed[4];
    /* 0x28 */ u8 pad28[0x3C - 0x28];
    /* 0x3C */ u8 dead;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct Actor {
    /* 0x000 */ u8 kind;                /* 1 = actor, 2 = particle */
    /* 0x001 */ u8 pad1[0xD4 - 0x1];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x10C - 0xD8];
    /* 0x10C */ struct Actor *particleOwner;
    /* 0x110 */ u8 pad110[0x1A8 - 0x110];
    /* 0x1A8 */ struct Player *owner;
} Actor;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x518 - 0x14];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[0x524 - 0x51C];
    /* 0x524 */ s32 unk524;
    /* 0x528 */ u8 pad528[0xBF4 - 0x528];
    /* 0xBF4 */ s32 team;
} Player;

typedef struct RegionInfo {
    /* 0x00 */ u8 pad0[0x52];
    /* 0x52 */ u16 flags;
} RegionInfo;

typedef struct Hit {
    /* 0x00 */ Actor *source;
    /* 0x04 */ s32 damage;
    /* 0x08 */ s32 pad8;
    /* 0x0C */ s32 unkC;
} Hit;

extern s32 D_8011AAD8;
extern u8 D_800F7078[];
extern u8 D_80119870[];
typedef struct TeamGame {
    /* 0x00 */ s32 active;
} TeamGame;

extern TeamGame D_8011ACD4;
extern s32 D_8011ACDC;      /* team 0 score */
extern s32 D_8011ACE0;      /* team 1 score */

s32 Cinema_PathPlaying(void);
s32 func_00284174(void);
RegionInfo *func_0022425C(void *world, void *region);
void func_00249358(Player *player, Actor *actor, s32 damage, s32 arg3, s32 arg4);
s32 func_0025340C(void *players, Player *player);
void func_002478C8(Player *victim, Player *killer, Hit *hit);

static __inline__ s32 inSpecialRegion(Player *player) {
    RegionInfo *info;

    if (player->region != NULL) {
        info = func_0022425C(D_800F7078, player->region);
        if (info != NULL) {
            if (info->flags & 0x40) {
                return 1;
            }
        }
    }
    return 0;
}

/* A player's actor took a lethal hit in multiplayer: works out the killer and
   updates the kill, death, tag and team counts. */
void func_00247C10(Actor *actor, s32 arg1, Hit *hit) {
    Player *player;
    Player *killer = NULL;
    s32 arg4 = -1;
    RegionInfo *info;
    s32 special;
    Actor *source;
    Actor *owner;
    PlayerOptions *options;
    s32 killerIndex;
    s32 victimIndex;
    s32 *score;
    s32 *teamScore;

    if (Cinema_PathPlaying() != 0) {
        return;
    }
    if (D_8011AAD8 & 1) {
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
    func_00249358(player, actor, hit->damage, hit->unkC, arg4);

    source = hit->source;
    if (source != NULL) {
        switch (source->kind) {
            case 2:
                owner = source->particleOwner;
                if (owner != NULL && owner->kind == 1 && (owner->flags & 0x300000)) {
                    killer = (Player *) owner;
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
    if (player->unk524 != 0) {
        return;
    }
    if (player->options->dead != 0) {
        return;
    }
    player->options->dead = 1;
    killerIndex = func_0025340C(D_80119870, killer);
    options = player->options;
    if (options->tagMode == 1) {
        options->taggedBy[killerIndex]++;
    } else {
        options->killedBy[killerIndex]++;
    }
    victimIndex = func_0025340C(D_80119870, player);
    if (player->options->tagMode == 1) {
        killer->options->tagged[victimIndex]++;
    } else {
        killer->options->killed[victimIndex]++;
    }
    func_002478C8(player, killer, hit);

    if (killerIndex == victimIndex) {
        player->options->suicides++;
        if (D_8011ACD4.active != 0) {
            switch (player->team) {
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
        player->options->deaths++;
        if (player->options->tagMode == 1) {
            killer->options->tags++;
            if (D_8011ACD4.active != 0) {
                switch (killer->team) {
                    case 0:
                        score = &D_8011ACDC;
                        break;
                    case 1:
                        score = &D_8011ACE0;
                        break;
                    default:
                        return;
                }
                if (player->team != killer->team) {
                    (*score)++;
                }
            }
        } else {
            killer->options->kills++;
            if (D_8011ACD4.active != 0) {
                switch (killer->team) {
                    case 0:
                        score = &D_8011ACDC;
                        break;
                    case 1:
                        score = &D_8011ACE0;
                        break;
                    default:
                        return;
                }
                if (player->team != killer->team) {
                    (*score)++;
                }
            }
        }
    }
}
