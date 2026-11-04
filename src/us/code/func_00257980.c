#include "common.h"
#include "structs/player.h"

/* The camera as read here: a word in its menu pool (CCamera.m_MenuPool + 0x10). */
typedef struct {
    /* 0x000 */ u8 pad0[0x54C];
    /* 0x54C */ s32 menuActive;
} CameraMenu;

/* Only what this reads and writes. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 started;
    /* 0x38 */ s32 unk38;
} CinemaState;

s32 Cinema_PathPlaying();                           /* extern */
s32 func_00284408();                                /* extern */
s32 func_00416AF8(void *);                          /* extern */
extern u8 D_80119E2C[];

void func_00257980(CinemaState *arg0, CPlayer *arg1) {
    CameraMenu *temp_a1;

    if ((func_00416AF8(D_80119E2C) == 0) && (Cinema_PathPlaying() != 0) && (func_00284408() == 0x190) && (arg0->started == 0)) {
        temp_a1 = (CameraMenu *) arg1->m_pCamera;
        if ((temp_a1 != NULL) && (temp_a1->menuActive == 0)) {
            arg0->started = 1;
            arg0->unk38 = 0;
        }
    }
}
