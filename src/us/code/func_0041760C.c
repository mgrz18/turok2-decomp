#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"

extern CPlayerOptions D_8011AB58[4]; /* D_8011AAD8.slots */

extern s32 Controller_ConfirmPressed(ControllerProfile *ctl);
extern s32 Random_Next(void);
extern void func_00430454(void *a, s32 level);

/* Mp_SetupMatchRules, inlined. */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    D_8011AAD8.match.tag = started;
    D_8011AAD8.match.teams = a3;
    D_8011AAD8.match.scoreLimit = a1;
    D_8011AAD8.match.over = 0;
    D_8011AAD8.match.teamScore[0] = 0;
    D_8011AAD8.match.teamScore[1] = 0;
    D_8011AAD8.match.teamOver = 0;
    D_8011AAD8.match.done = 0;
    D_8011AAD8.match.timeLeft = minutes * 900.0f;
    if (D_8011AAD8.match.timeLeft == 0.0f) {
        D_8011AAD8.match.timeLeft = -1.0f;
    }
}

s32 MpMenu_StartBloodLust(void) {
    GameSettings *setup = &D_8011AAD8;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->dmArena;
    StartMatch(setup->dmTimeLimit, setup->dmScoreLimit, 0, 0);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = D_800F5EC0; i < 4; i++, ctl++) {
        if (Controller_ConfirmPressed(ctl)) {
            CPlayerOptions *slot = &D_8011AB58[i];

            slot->m_bActive = 1;
            slot->m_nController = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 13) {
            arg = level + 0x2328;
        } else {
            arg = Random_Next() % 13 + 0x2329;
        }
    } else {
        arg = Random_Next() % 13 + 0x2329;
    }
    func_00430454(&gEngineApp, arg);
    return 1;
}
