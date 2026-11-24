#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"


/* Turns the object by arg1 and wraps its angle. */
void Actor_TurnBy(CGameObjectInstance *arg0, f32 arg1) {
    arg0->m_RotY = (f32) (arg0->m_RotY + arg1);
    Angle_Wrap(&arg0->m_RotY);
}
