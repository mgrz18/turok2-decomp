/* Second-module copy of func_0041778C (build/manual/W/func_0041778C.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"

extern CPlayerOptions D_8011AB58[4]; /* D_8011AAD8.slots */

extern s32 func_002855E8(ControllerProfile *ctl);
extern s32 func_002113A4(void);
extern void func_00430374(void *a, s32 level);

/* func_00417BB0, inlined. */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    GameMatch *m = &D_8011AAD8.match;

    m->tag = started;
    m->teams = a3;
    m->scoreLimit = a1;
    m->over = 0;
    m->teamScore[0] = 0;
    m->teamScore[1] = 0;
    m->teamOver = 0;
    m->done = 0;
    m->timeLeft = minutes * 900.0f;
    if (m->timeLeft == 0.0f) {
        m->timeLeft = -1.0f;
    }
}

s32 func_0045078C(void) {
    GameSettings *setup = &D_8011AAD8;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->teamArena;
    StartMatch(setup->teamTimeLimit, setup->teamScoreLimit, 0, 1);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = D_800F5EC0; i < 4; i++, ctl++) {
        if (func_002855E8(ctl)) {
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
            arg = func_002113A4() % 13 + 0x2329;
        }
    } else {
        arg = func_002113A4() % 13 + 0x2329;
    }
    func_00430374(&D_800F6CB0, arg);
    return 1;
}
