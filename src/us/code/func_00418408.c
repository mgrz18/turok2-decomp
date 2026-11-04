#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* The menu this handler belongs to: only its player here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} PlayerMenu;

s32 func_00416644(s32, s32, s32, s32, s32);
extern u8 D_00438808[];

s32 func_00418408(s32 arg0, PlayerMenu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 1;
    temp_a2->m_pOptions->m_nCharacter = 0;
    func_00416644((s32) temp_a2->m_pCamera->m_MenuPool, (s32) D_00438808, (s32) temp_a2, temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}
