#include "common.h"

/* A player (0x1324 bytes, chained through 0x1320); only the float at 0xA28 is set here. */
typedef struct Player {
    /* 0x0000 */ u8 pad0[0xA28];
    /* 0x0A28 */ f32 unkA28;
    /* 0x0A2C */ u8 padA2C[0x1320 - 0xA2C];
    /* 0x1320 */ struct Player *next;
} Player;

/* The player list (D_80119870): the first player at 0x20. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *first;
} PlayerList;

void func_002535E4(PlayerList *arg0, f32 arg1) {
    Player *var_v0;

    var_v0 = arg0->first;
    if (var_v0 != NULL) {
        do {
            var_v0->unkA28 = arg1;
            var_v0 = var_v0->next;
        } while (var_v0 != NULL);
    }
}
