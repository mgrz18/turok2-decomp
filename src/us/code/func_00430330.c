/* WRONG CUT: the real function is func_00430330, size 0x7C (splat split it at func_00430374). */
#include "common.h"

/* Turok 2: the mode block sits at 0x23FD0 (see func_00288E58). */
typedef struct GameSession {
    char pad0[0x23FDC];
    s32 target;                /* 0x23FDC */
    char pad23FE0;
    s8 phase;                  /* 0x23FE1 */
} GameSession;

typedef struct MatchState {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0xC];
    /* 0x34 */ s32 split;
    /* 0x38 */ u8 pad38[0xC];
    /* 0x44 */ s32 unk44;
} MatchState;

extern char D_800F5A50[];
extern char D_80119870[];
extern MatchState D_8011ACB0;

void func_00285410(s32);
void func_0026EDA8(void *);
void func_0025359C(void *);

/* Resets the match state and asks the session for mode 1. */
void func_00430330(GameSession *session) {
    MatchState *m;

    func_00285410(0);
    func_0026EDA8(D_800F5A50);
    func_0025359C(D_80119870);
    m = &D_8011ACB0;
    m->unk24 = 0;
    m->split = 0;
    m->unk44 = 0;
    session->phase = 2;
    session->target = 1;
}
