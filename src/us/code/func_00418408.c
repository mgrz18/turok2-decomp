#include "common.h"
#include "structs/camera.h"

/* Multiplayer options kept per player (see func_00247C10.c): only 0x30. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s8 unk30;
} PlayerOptions;

/* The player, as read here (CPlayer's header is off by a few words from
 * 0xA08 on; 0xBF4 is the team as in func_00247C10.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0x514];
    /* 0x514 */ s32 controller;
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ CCamera *camera;
    /* 0x520 */ u8 pad520[0xA50 - 0x520];
    /* 0xA50 */ s32 unkA50;
    /* 0xA54 */ u8 padA54[0xBF4 - 0xA54];
    /* 0xBF4 */ s32 team;
} Player;

/* The menu this handler belongs to: only its player here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
} PlayerMenu;

s32 func_00416644(s32, s32, s32, s32, s32);
extern u8 D_00438808[];

s32 func_00418408(s32 arg0, PlayerMenu *arg1) {
    Player *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->team = 1;
    temp_a2->options->unk30 = 0;
    func_00416644((s32) temp_a2->camera->m_MenuPool, (s32) D_00438808, (s32) temp_a2, temp_a2->unkA50, temp_a2->controller);
    return 1;
}
