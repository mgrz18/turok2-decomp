#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x53C];
    /* 0x53C */ u8 cinema[1];
} Camera;

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

void func_0042EB04(CPlayer *);
s32 func_0025340C(Players *players, CPlayer *player);
void func_00416644(void *pool, void *screen, CPlayer *p, void *controller, s32);

/* func_0042EB04 takes the player (as in func_00249118): the call keeps it in $a0,
 * which is what lets sched2 keep the mask's lui/ori together. */
/* Clears the player's flag 0x800000 and, in multiplayer, opens the screen of
 * the player's slot (one of four) on the player's camera. */
void func_00255D70(CPlayer *player) {
    char *screen;

    player->m_ShockedTimer = 0;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    func_0042EB04(player);
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
        func_00416644(((Camera *)player->m_pCamera)->cinema, screen, player, player->m_ContState.m_pContState, 0);
    }
}
