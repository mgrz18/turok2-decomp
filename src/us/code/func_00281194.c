#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;     /* the players, a list through m_pNext */

/* The player whose camera this is, or NULL. */
CPlayer *CCamera__GetPlayer(CCamera *cam) {
    CPlayer *player;

    for (player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            return player;
        }
    }
    return NULL;
}
