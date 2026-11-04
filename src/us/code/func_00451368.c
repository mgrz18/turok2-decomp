#include "common.h"

/* The player options (CPlayer.m_pOptions; D_8011AB18 holds the defaults). */
struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x2A];
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 pad2B[2];
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ s8 unk30;
    /* 0x31 */ u8 pad31[0x3D - 0x31];
    /* 0x3D */ s8 unk3D;
};

/* The player, laid out as include/structs/player.h up to 0x520 (past 0xA00 that
 * header does not line up with the code, so those fields stay unnamed). */
typedef struct {
    /* 0x000 */ u8 pad0[0x514];
    /* 0x514 */ s32 m_nController;
    /* 0x518 */ struct CPlayerOptions *m_pOptions;
    /* 0x51C */ s32 m_pCamera;
    /* 0x520 */ u8 pad520[0xA50 - 0x520];
    /* 0xA50 */ s32 unkA50;
    /* 0xA54 */ u8 padA54[0xBF4 - 0xA54];
    /* 0xBF4 */ s32 unkBF4;
} Player;

/* A menu item (MenuItem in func_00450F4C): the player whose options it shows. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
} MenuItem;

s32 func_00416644(s32, s32, s32, s32, s32);

extern s32 D_00438728;

s32 func_00451368(s32 arg0, MenuItem *arg1) {
    Player *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->unkBF4 = 1;
    temp_a2->m_pOptions->unk30 = 0;
    func_00416644(temp_a2->m_pCamera + 0x53C, (s32) &D_00438728, (s32) temp_a2, temp_a2->unkA50, temp_a2->m_nController);
    return 1;
}
