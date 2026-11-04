#include "common.h"

/* A play-time clock: frames, seconds, minutes, hours. */
typedef struct {
    /* 0x0 */ f32 frames;
    /* 0x4 */ f32 seconds;
    /* 0x8 */ f32 minutes;
    /* 0xC */ f32 hours;
} PlayClock;

/* Only the field read here. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FF4];
    /* 0x23FF4 */ s32 paused;
} BigState;

extern f32 D_800A9DF4;                              /* units per next unit (presumably 60) */
extern f32 D_800B6D28;                              /* frame delta */
extern PlayClock D_8011ACB0;

void func_00288FDC(BigState *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;

    if (arg0->paused == 0) {
        temp_f1 = D_8011ACB0.frames + D_800B6D28;
        D_8011ACB0.frames = temp_f1;
        if (temp_f1 >= 15.0f) {
            temp_f0 = D_8011ACB0.seconds + 1.0f;
            D_8011ACB0.frames = (f32) (temp_f1 - 15.0f);
            D_8011ACB0.seconds = temp_f0;
            if (D_800A9DF4 <= temp_f0) {
                temp_f0_2 = D_8011ACB0.minutes + 1.0f;
                D_8011ACB0.seconds = (f32) (temp_f0 - D_800A9DF4);
                D_8011ACB0.minutes = temp_f0_2;
                if (D_800A9DF4 <= temp_f0_2) {
                    D_8011ACB0.minutes = (f32) (temp_f0_2 - D_800A9DF4);
                    D_8011ACB0.hours = (f32) (D_8011ACB0.hours + 1.0f);
                }
            }
        }
    }
}
