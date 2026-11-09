#include "common.h"

s32 RumblePool_FreeAll(s32);                         /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[0xD0];
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ u8 padD4[0x140 - 0xD4];
    /* 0x140 */ u8 sub140[1];
} Attached002534E0;

/* 0x1328-byte list entries (see CPlayerPool__GetIndex) linked through 0x1320. */
typedef struct Node002534E0 {
    /* 0x0000 */ u8 pad0[0xA50];
    /* 0x0A50 */ Attached002534E0 *attached;
    /* 0x0A54 */ u8 padA54[0x1320 - 0xA54];
    /* 0x1320 */ struct Node002534E0 *next;
} Node002534E0;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Node002534E0 *head;
} List002534E0;

void func_0025359C(List002534E0 *arg0) {
    Node002534E0 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            RumblePool_FreeAll((sptr) var_s0->attached->sub140);
            var_s0->attached->unkD0 = 0;
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
