#include "common.h"

s32 CPlayer__LoseLife(s32);                         /* extern */
extern s32 D_800F1FD8;

void PlayerCinema50_Done(void) {
    CPlayer__LoseLife(D_800F1FD8);
}
