#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

void CMtxF__TransformPoint(CMtxF, CVector3 *, CVector3 *);

/* Transforms a point by the player camera's view orientation, in place. */
void CPlayer__TransformByView(CPlayer *player, CVector3 *pos) {
    CVector3 in = *pos;

    CMtxF__TransformPoint(&player->m_pCamera->m_mfViewOrient, &in, pos);
}
