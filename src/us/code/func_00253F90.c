#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);

/* Transforms a point by the player camera's view orientation, in place. */
void CPlayer__TransformByView(CPlayer *player, CVector3 *pos) {
    CVector3 in = *pos;

    CMtxF__TransformPoint(&player->m_pCamera->m_mfViewOrient, &in, pos);
}
