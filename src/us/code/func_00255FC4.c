#include "common.h"

s32 Cinema_SetNextCallbacks(s32, s32, s32 *); /* extern */
extern s32 D_800F1FD8;
extern s32 PlayerCinema50_Done;

void PlayerCinema50_Enter(s32 arg0) {
    D_800F1FD8 = arg0;
    Cinema_SetNextCallbacks(0x50, 0, &PlayerCinema50_Done);
}
