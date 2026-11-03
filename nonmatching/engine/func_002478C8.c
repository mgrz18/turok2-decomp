/* NON-MATCHING: 134 of 210 words differ (size 0x2fc vs 0x348): everything up to the tag branch's killer message matches (the victim's 1.5f in $f20 comes from the block-local `time`, the `s2 - 0x1228` game pointer from the block-local `game`). The ROM keeps the tag branch's killer message (a0 = s2) apart; here jump2's cross-jumping merges it with the "you killed" tail, because the a0 moves of both carry a REG_EQUAL D_801198B0 note (find_cross_jump compares notes). In the ROM the tag branch's a0 move must carry no constant note; the permuter only splits them with fake temporaries. */
#include "common.h"
#include "structs/player.h"

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

typedef struct MatchSettings {
    /* 0x00 */ u8 pad0[0x19];
    /* 0x19 */ u8 messages;         /* show kill messages */
} MatchSettings;

/* The global at D_801198B0: what this reads of it. */
typedef struct Game {
    /* 0x0000 */ u8 pad0[0x1228];
    /* 0x1228 */ MatchSettings settings;    /* D_8011AAD8 */
} Game;

extern Game D_801198B0;

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
    MatchSettings *settings = &D_801198B0.settings;
    HitTarget *target;

    if (settings->messages != 0) {
        if (victim == killer) {
            func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], "you killed yourself");
            SHOW_MESSAGE(&D_801198B0, killer, 1.5f);
        } else if (victim->m_pOptions->tagMode != 0) {
            f32 time = 1.5f;
            Game *game;

            func_00288800(victim->m_DeathMatchString[victim->m_nDeathmatchString], "tagged by ");
            func_00288800(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->name);
            game = &D_801198B0;
            SHOW_MESSAGE(game, victim, time);
            func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], "you tagged ");
            func_00288800(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->name);
            SHOW_MESSAGE(game, killer, 1.5f);
        } else {
            f32 time = 1.5f;
            Game *game;

            func_00288800(victim->m_DeathMatchString[victim->m_nDeathmatchString], "killed by ");
            func_00288800(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->name);
            game = &D_801198B0;
            SHOW_MESSAGE(game, victim, time);
            target = hit->target;
            if (target != NULL && (target->region & 3) == 1) {
                func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], "head shot!");
                SHOW_MESSAGE(game, killer, 1.5f);
            } else {
                func_00288800(killer->m_DeathMatchString[killer->m_nDeathmatchString], "you killed ");
                func_00288800(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->name);
                SHOW_MESSAGE(&D_801198B0, killer, 1.5f);
            }
        }
    }
}
