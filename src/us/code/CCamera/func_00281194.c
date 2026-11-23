#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"


/* The player whose camera this is, or NULL. */
CPlayer *CCamera__GetPlayer(CCamera *cam) {
    CPlayer *player;

    for (player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            return player;
        }
    }
    return NULL;
}
