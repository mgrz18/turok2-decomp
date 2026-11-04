#include "common.h"
#include "structs/game.h"

extern GameSlot D_8011AB58[4]; /* D_8011AAD8.slots */
extern u8 D_800F5EC0[];         /* controllers, 0x224 bytes each */
extern u8 D_800F6CB0[];

extern s32 func_002855E8(void *ctl);
extern s32 func_002113A4(void);
extern void func_00430454(void *a, s32 level);

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

s32 func_00417490(void) {
    GameSettings *setup = &D_8011AAD8;
    s32 level;
    s32 arg;
    s32 i;
    u8 *ctl;

    level = setup->tagArena;
    StartMatch(setup->tagTimeLimit, setup->tagScoreLimit, 1, 0);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].character = 0;
    }
    for (i = 0, ctl = D_800F5EC0; i < 4; i++, ctl += 0x224) {
        if (func_002855E8(ctl)) {
            GameSlot *slot = &D_8011AB58[i];

            slot->active = 1;
            slot->controller = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 6) {
            arg = level + 0x2336;
        } else {
            arg = func_002113A4() % 6 + 0x2337;
        }
    } else {
        arg = func_002113A4() % 6 + 0x2337;
    }
    func_00430454(D_800F6CB0, arg);
    return 1;
}
