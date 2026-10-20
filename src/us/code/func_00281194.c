#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern CPlayer *D_80119890;     /* the players, a list through m_pNext */

/* The player whose camera this is, or NULL. */
CPlayer *func_00281194(CCamera *cam) {
    CPlayer *player;

    for (player = D_80119890; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            return player;
        }
    }
    return NULL;
}
