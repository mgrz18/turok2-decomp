/* NON-MATCHING: 3 of 53 words differ: prologue scheduling only; the ROM puts the `ori` of the ~0x800000 mask right after its `lui`, before `sw $ra` and the flags load. */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x53C];
    /* 0x53C */ u8 cinema[1];
} Camera;

typedef struct {
    /* 0x0000 */ u8 pad0[0xD4];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0x51C - 0xD8];
    /* 0x051C */ Camera *camera;
    /* 0x0520 */ u8 pad520[0xA50 - 0x520];
    /* 0x0A50 */ void *controller;
    /* 0x0A54 */ u8 padA54[0x1298 - 0xA54];
    /* 0x1298 */ s32 unk1298;
} Player;

/* The player lists; the multiplayer flag is at +0x1281. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x1281];
    /* 0x1281 */ u8 multiplayer;
} Players;

extern Players D_80119870;
extern char D_0043ABF0;
extern char D_0043AC14;
extern char D_0043AC38;
extern char D_0043AC5C;

void func_0042EB04(void);
s32 func_0025340C(Players *players, Player *player);
void func_00416644(void *pool, void *screen, Player *p, void *controller, s32);

/* Clears the player's flag 0x800000 and, in multiplayer, opens the screen of
 * the player's slot (one of four) on the player's camera. */
void func_00255D70(Player *player) {
    char *screen;

    player->unk1298 = 0;
    player->flags &= ~0x800000;
    func_0042EB04();
    if (D_80119870.multiplayer) {
        switch ((u32)func_0025340C(&D_80119870, player)) {
        case 0:
        default:
            screen = &D_0043ABF0;
            break;
        case 1:
            screen = &D_0043AC14;
            break;
        case 2:
            screen = &D_0043AC38;
            break;
        case 3:
            screen = &D_0043AC5C;
            break;
        }
        func_00416644(player->camera->cinema, screen, player, player->controller, 0);
    }
}
