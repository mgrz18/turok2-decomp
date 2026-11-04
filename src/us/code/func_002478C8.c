#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x33];
    /* 0x33 */ char name[0x3E - 0x33];
    /* 0x3E */ u8 tagMode;
} CPlayerOptions;

typedef struct HitTarget {
    /* 0x00 */ u8 pad0[0x65];
    /* 0x65 */ u8 region;           /* low two bits: 1 = head */
} HitTarget;

typedef struct Hit {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ HitTarget *target;
} Hit;

/* The global at D_801198B0: what this reads of it. */
typedef struct Game {
    /* 0x0000 */ u8 pad0[0x1228];
    /* 0x1228 */ GameSettings settings;    /* D_8011AAD8 */
} Game;

extern Game D_801198B0;

/* The game the settings belong to, recovered from the settings pointer. Its address is
   D_8011AAD8 - 0x1228 for the compiler, not D_801198B0: that is what keeps the tagged and
   head-shot messages from being cross-jumped into the "you killed" one. */
#define GAME_OF(settings) ((Game *)((u8 *)(settings) - 0x1228))

/* The messages live in the data after .rodata ("head shot!" in the hit-region names), not in this file's pool. */
extern char D_800B53D0[]; /* "head shot!" */
extern char D_800B53FC[]; /* "tagged by " */
extern char D_800B5408[]; /* "you tagged " */
extern char D_800B5414[]; /* "killed by " */
extern char D_800B5420[]; /* "you killed " */
extern char D_800B542C[]; /* "you killed yourself" */

char *func_00288800(char *dst, char *src); /* strcpy */
void func_00281610(Game *game, struct CCamera *camera, char *text, f32 time);

/* Shows a player's newest message (ring of four) on its camera. */
#define SHOW_MESSAGE(game, player, time)                                                         \
    func_00281610(game, (player)->m_pCamera,                                                     \
                  (player)->m_DeathMatchString[(player)->m_nDeathmatchString], time);            \
    (player)->m_nDeathmatchString++;                                                             \
    (player)->m_nDeathmatchString %= 4;

/* Deathmatch messages for a kill: "killed by" or "tagged by" on the victim's
   screen, "you killed", "you tagged" or "head shot!" on the killer's. */
void func_002478C8(CPlayer *victim, CPlayer *killer, Hit *hit) {
    GameSettings *settings = &D_8011AAD8;
    HitTarget *target;

    if (settings->multiplayer != 0) {
        if (victim == killer) {
            func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], D_800B542C);
            SHOW_MESSAGE(&D_801198B0, killer, 1.5f);
        } else if (victim->m_pOptions->tagMode != 0) {
            f32 time = 1.5f;
            Game *game;

            func_00288800(victim->m_DeathMatchString[victim->m_nDeathmatchString], D_800B53FC);
            func_00288800(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->name);
            game = GAME_OF(settings);
            SHOW_MESSAGE(game, victim, time);
            func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], D_800B5408);
            func_00288800(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->name);
            SHOW_MESSAGE(game, killer, 1.5f);
        } else {
            f32 time = 1.5f;
            Game *game;

            func_00288800(victim->m_DeathMatchString[victim->m_nDeathmatchString], D_800B5414);
            func_00288800(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->name);
            game = GAME_OF(settings);
            SHOW_MESSAGE(game, victim, time);
            target = hit->target;
            if (target != NULL && (target->region & 3) == 1) {
                func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], D_800B53D0);
                SHOW_MESSAGE(game, killer, 1.5f);
            } else {
                func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], D_800B5420);
                func_00288800(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->name);
                SHOW_MESSAGE(&D_801198B0, killer, 1.5f);
            }
        }
    }
}
