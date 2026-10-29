/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292910.c, func_00292910, with Turok 2's layouts. */
#include "common.h"

typedef struct Level {
    /* 0x00000 */ u8 pad0[0x2317C];
    /* 0x2317C */ u8 unk2317C[0x23FD0 - 0x2317C];
    /* 0x23FD0 */ f32 spin;
    /* 0x23FD4 */ s32 prevState;
    /* 0x23FD8 */ s32 state;
    /* 0x23FDC */ s32 nextState;
    /* 0x23FE0 */ u8 fastFade;
    /* 0x23FE1 */ u8 fadeMode;
    /* 0x23FE2 */ u8 pad23FE2[0x23FF4 - 0x23FE2];
    /* 0x23FF4 */ s32 paused;
} Level;

typedef struct VideoRate {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 hz;
} VideoRate;

extern u64 D_8011AD08;          /* time of the last frame */
extern s32 D_8011AD10;          /* retraces this frame */
extern u32 D_8011B09C;          /* retrace counts of the last four frames */
extern u32 D_8011B10C;
extern u32 D_800F6CA8;
extern u32 D_8011AD14;
extern u32 D_801198A0;
extern s32 D_801308F8;
extern s32 D_800C2034;          /* screen height */
extern VideoRate D_8011B0A0;
extern f32 D_800B6D28;          /* frame time */
extern void *D_800F7070;
extern void *D_8011F140;
extern s32 D_801198A8;
extern s32 D_80130940;
extern u8 D_80119E2C[];
extern s32 D_8011ACF4;

extern u64 func_0029B170(void);     /* osGetTime */
extern s32 func_00416AF8(void *);
extern s32 func_0027BC34(void);
extern s32 Cinema_PathPlaying(void);
extern void func_0027BC68(void);
extern void func_00204DE0(s32);
extern void func_00287248(Level *);
extern void func_00288FDC(Level *);
extern s32 func_00284408(void);
extern void func_00412B14(void);
extern s32 func_00284240(void);

/* Retraces per frame: the mean of the last four frames without the slowest, at least `min`. */
#define SLOWEST ({ u32 _m = a; if (_m < d) _m = d; if (_m < c) _m = c; if (_m < b) _m = b; _m; })
#define MEAN ((sum - ({ u32 _t = SLOWEST; _t - 2; })) / 3)

static __inline__ u32 frameRetraces(u32 min) {
    u32 a = D_8011B09C;
    u32 b = D_8011B10C;
    u32 c = D_800F6CA8;
    u32 d = D_8011AD14;
    u32 sum = b + c + d + a;

    return MEAN < min ? min : MEAN;
}

/* Runs one frame of the level: timing, draw, then the pause/attract checks. */
void func_00287ABC(Level *level) {
    u64 now;
    f32 elapsed;
    s32 quit;

    now = func_0029B170();
    elapsed = (f32)((now - D_8011AD08) * 64 / 3000) * 0.000001f * 15.0f;
    D_8011AD08 = now;
    if (level->state == 0xD) {
        level->paused = func_00416AF8(level->unk2317C);
    } else {
        level->paused = 0;
    }
    if (D_801308F8 != 0 && func_0027BC34() == 0 && level->state == 0xB) {
        level->paused = 1;
    }
    switch ((u32)level->state) {
    case 1:
    case 3:
    case 4:
        D_8011AD10 = frameRetraces(2);
        break;
    default:
        D_8011AD10 = frameRetraces(2);
        if (D_801198A0 >= 3) {
            D_8011AD10 = frameRetraces(3);
        }
        if (Cinema_PathPlaying() != 0) {
            if ((u32)(level->state - 0xF) < 2 && D_800C2034 < 0xDF) {
                D_8011AD10 = frameRetraces(2);
            } else {
                D_8011AD10 = frameRetraces(4);
            }
        }
        break;
    }
    func_0027BC68();
    D_800B6D28 = (f32)(D_8011AD10 * 15) / D_8011B0A0.hz;
    if (Cinema_PathPlaying() != 0) {
        D_800B6D28 = elapsed;
    }
    if (func_0027BC34() != 0) {
        D_800B6D28 = (f32)(D_8011AD10 * 15) * (1.0f / 60.0f);
    }
    func_00204DE0(0);
    D_8011F140 = D_800F7070;
    if (D_801198A8 == 0) {
        D_80130940 = 0;
    }
    func_00287248(level);
    level->spin += D_800B6D28;
    func_00288FDC(level);
    if (func_00416AF8(D_80119E2C) == 0 && Cinema_PathPlaying() != 0 && func_00284408() == 0x78) {
        if (D_8011ACF4 == 0 && level->paused == 0) {
            D_8011ACF4 = 1;
            func_00412B14();
        }
    }
    if (D_8011ACF4 != 0 && func_00284240() != 0) {
        quit = 1;
    } else {
        quit = 0;
    }
    if (quit && level->state != 0xF && level->nextState != 0xF) {
        level->fadeMode = 2;
        level->nextState = 0xF;
    }
}
