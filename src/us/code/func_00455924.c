#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

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

s32 func_00455924(s32 arg0, Menu *arg1) {
    ((CPlayer *) arg1->player)->m_pOptions->unk3D = 1;
    return 1;
}
