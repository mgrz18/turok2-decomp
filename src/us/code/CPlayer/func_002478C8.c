#include "common.h"
#include "globals/camera.h"
#include "globals/strings.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/collision.h"
#include "globals/game.h"

#define CAMERA_POOL_GAME (*(CGameFromCameraPool *) &gEngineApp_m_Game_m_CameraPool) /* the file's view of gEngineApp_m_Game_m_CameraPool, which globals/camera.h declares: this type is what keeps the ROM's code */

/* The game the settings belong to, recovered from the settings pointer. Its address is
   gEngineApp_m_Settings - 0x1228 for the compiler, not gEngineApp_m_Game_m_CameraPool: that is what keeps the tagged and
   head-shot messages from being cross-jumped into the "you killed" one. */
#define GAME_OF(settings) ((CGameFromCameraPool *)((u8 *)(settings) - T2_OFFSETOF(CGameFromCameraPool, m_Settings)))

/* The messages live in the data after .rodata ("head shot!" in the hit-region names), not in this file's pool. */

char *Util_StrCpy(char *dst, char *src); /* strcpy */
void CCameraPool__AddTimedText(CGameFromCameraPool *game, struct CCamera *camera, char *text, f32 time);

/* Shows a player's newest message (ring of four) on its camera. */
#define SHOW_MESSAGE(game, player, time)                                                         \
    CCameraPool__AddTimedText(game, (player)->m_pCamera,                                                     \
                  (player)->m_DeathMatchString[(player)->m_nDeathmatchString], time);            \
    (player)->m_nDeathmatchString++;                                                             \
    (player)->m_nDeathmatchString %= 4;

/* Deathmatch messages for a kill: "killed by" or "tagged by" on the victim's
   screen, "you killed", "you tagged" or "head shot!" on the killer's. */
void CPlayer__ShowKillMessages(CPlayer *victim, CPlayer *killer, DamageEvent *hit) {
    GameSettings *settings = &gEngineApp_m_Settings;
    HitFrame *target;

    if (settings->multiplayer != 0) {
        if (victim == killer) {
            Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_YouKilledYourself);
            SHOW_MESSAGE(&CAMERA_POOL_GAME, killer, 1.5f);
        } else if (victim->m_pOptions->m_TagMode != 0) {
            f32 time = 1.5f;
            CGameFromCameraPool *game;

            Util_StrCpy(victim->m_DeathMatchString[victim->m_nDeathmatchString], sKillMessageText_TaggedBy);
            Util_StrCpy(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->m_Name);
            game = GAME_OF(settings);
            SHOW_MESSAGE(game, victim, time);
            Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_YouTagged);
            Util_StrCpy(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->m_Name);
            SHOW_MESSAGE(game, killer, 1.5f);
        } else {
            f32 time = 1.5f;
            CGameFromCameraPool *game;

            Util_StrCpy(victim->m_DeathMatchString[victim->m_nDeathmatchString], sKillMessageText_KilledBy);
            Util_StrCpy(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->m_Name);
            game = GAME_OF(settings);
            SHOW_MESSAGE(game, victim, time);
            target = hit->frame;
            if (target != NULL && (target->flags & 3) == 1) {
                Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_HeadShot);
                SHOW_MESSAGE(game, killer, 1.5f);
            } else {
                Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_YouKilled);
                Util_StrCpy(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->m_Name);
                SHOW_MESSAGE(&CAMERA_POOL_GAME, killer, 1.5f);
            }
        }
    }
}

#undef CAMERA_POOL_GAME
