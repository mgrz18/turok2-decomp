#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Only what this reads and writes. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 started;
    /* 0x38 */ s32 unk38;
} CinemaState;

s32 Cinema_PathPlaying();                           /* extern */
s32 Cinema_GetScriptId();                                /* extern */
s32 MenuPool_CountOpen(void *);                          /* extern */
extern u8 D_80119E2C[];

void Cinema_CheckPlayerStart(CinemaState *arg0, CPlayer *arg1) {
    CCamera *temp_a1;

    if ((MenuPool_CountOpen(D_80119E2C) == 0) && (Cinema_PathPlaying() != 0) && (Cinema_GetScriptId() == 0x190) && (arg0->started == 0)) {
        temp_a1 = arg1->m_pCamera;
        if ((temp_a1 != NULL) && (temp_a1->m_Menus.count == 0)) {
            arg0->started = 1;
            arg0->unk38 = 0;
        }
    }
}
