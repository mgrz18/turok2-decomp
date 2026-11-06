#include "common.h"
#include "structs/game.h"

#define TEAM(p) (*(s32 *) ((char *) (p) + 0xBF4))

void CPlayerPool__AddTeamScore(char *player, char *other, s32 event) {
    GameMatch *score = &D_8011ACB0;
    s32 *count;

    if (score->teams == 0) {
        return;
    }
    switch (TEAM(player)) {
        case 0:
            count = &score->teamScore[0];
            break;
        case 1:
            count = &score->teamScore[1];
            break;
        default:
            return;
    }
    switch (event) {
        case 0:
            (*count)--;
            break;
        case 1:
            if (TEAM(other) != TEAM(player)) {
                (*count)++;
            }
            break;
        case 2:
            if (TEAM(other) != TEAM(player)) {
                (*count)++;
            }
            break;
    }
}
