#include "common.h"

s32 CPlayer__LoseLife(s32);                         /* extern */
extern s32 sCinema50Player;

void PlayerCinema50_Done(void) {
    CPlayer__LoseLife(sCinema50Player);
}
