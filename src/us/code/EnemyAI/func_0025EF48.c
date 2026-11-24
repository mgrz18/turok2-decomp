#include "common.h"
#include "protos/enemy_ai.h"

s32 Cinema_RequestForObject(sptr, sptr);                /* extern */

void EnemyAI_EventPlayCinemaRemote(s32 arg0) {
    Cinema_RequestForObject(0, arg0);
}
