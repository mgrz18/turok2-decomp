/* Second VM module copy of func_0042E894 (same source, linked 0x0042E7B4). */
#include "common.h"

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x51C];
    /* 0x051C */ s32 unk51C;
    /* 0x0520 */ u8 pad520[0x1320 - 0x520];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct PlayerList {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

extern s32 D_8011AC88; /* game state */

extern s32 func_0028D0E0(void);
extern void func_0028D0F0(void);
extern void func_0028D100(void);
extern void func_0042DD94(Player *p);
extern void func_0024CF58(Player *p, Player *q);
extern void func_00284254(s32);

void func_004677B4(PlayerList *list) {
    Player *p;

    if (D_8011AC88 == 13 && func_0028D0E0() != 0) {
        func_0028D100();
        func_0028D0F0();
    }
    for (p = list->head; p != NULL; p = p->next) {
        func_0042DD94(p);
        func_0024CF58(p, p);
        func_00284254(p->unk51C);
    }
}
