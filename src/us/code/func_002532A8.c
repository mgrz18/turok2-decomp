#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

s32 func_002532A8(PlayerList *arg0) {
    return (s32) arg0->head;
}
