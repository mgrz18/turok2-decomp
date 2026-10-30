/* NON-MATCHING: 18 of 42 words differ: $s0/$s1 swapped (session/warp) and the scheduler puts the phase store ahead of the 0x23FFC/0x23FF8 stores. */
#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct WarpPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 angle;
    /* 0x10 */ s16 level;
    /* 0x12 */ s16 room;
} WarpPoint; /* size 0x14 */

/* Turok 2: the mode block sits at 0x23FD0 (see func_00288E58). */
typedef struct GameSession {
    char pad0[0x23FDC];
    s32 target;                /* 0x23FDC */
    char pad23FE0;
    s8 phase;                  /* 0x23FE1 */
    char pad23FE2[0x23FF8 - 0x23FE2];
    s32 warp;                  /* 0x23FF8 */
    s32 unk23FFC;              /* 0x23FFC */
} GameSession;

typedef struct MatchState {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x44 - 0x10];
    /* 0x44 */ s32 unk44;
} MatchState;

extern char D_800F7078[];
extern MatchState D_8011ACB0;

s32 func_0042C194(void *world, s32 arg1, s32 id, WarpPoint *out, s32 max);

/* Resets the match state and asks the session for mode 13 at warp `warp` (0 if the
 * world has no such warp point). */
void func_00430454(GameSession *session, s32 warp) {
    WarpPoint point;

    if (func_0042C194(D_800F7078, -1, warp, &point, 1) == 0) {
        warp = 0;
    }
    D_8011ACB0.unk0 = 0;
    D_8011ACB0.unk4 = 0;
    D_8011ACB0.unk8 = 0;
    D_8011ACB0.unkC = 0;
    D_8011ACB0.unk44 = 0;
    session->unk23FFC = 0;
    session->warp = warp;
    session->phase = 2;
    session->target = 13;
}
