#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern void func_0020F85C(CMtxF *m, CVector3 *in, CVector3 *out);

/* Transforms a point by the player camera's view orientation, in place. */
void func_00253F90(CPlayer *player, CVector3 *pos) {
    CVector3 in = *pos;

    func_0020F85C(&player->m_pCamera->m_mfViewOrient, &in, pos);
}
