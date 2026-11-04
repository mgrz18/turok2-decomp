#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3F];
    /* 0x3F */ u8 unk3F;
} Settings;

typedef struct {
    /* 0x000 */ u8 pad0[0x54C];
    /* 0x54C */ s32 unk54C;
} Owner;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x518];
    /* 0x0518 */ Settings *settings;
    /* 0x051C */ Owner *owner;
    /* 0x0520 */ u8 pad520[0x1320 - 0x520];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

/* Counts the players whose owner has unk54C clear and settings unk3F clear. */
s32 func_00254B9C(PlayerList *arg0) {
    s32 var_a0;
    Owner *temp_v0;
    Player *var_v1;

    var_v1 = arg0->head;
    var_a0 = 0;
    if (var_v1 != NULL) {
        do {
            temp_v0 = var_v1->owner;
            if ((temp_v0 != NULL) && (temp_v0->unk54C == 0) && (var_v1->settings->unk3F == 0)) {
                var_a0 += 1;
            }
            var_v1 = var_v1->next;
        } while (var_v1 != NULL);
    }
    return var_a0;
}
