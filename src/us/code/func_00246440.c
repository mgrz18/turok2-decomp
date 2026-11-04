#include "common.h"
#include "structs/game_object.h"

void func_00210EF0(f32 *);                          /* extern */

/* Turns the object by arg1 and wraps its angle. */
void func_00246440(CGameObjectInstance *arg0, f32 arg1) {
    arg0->m_RotY = (f32) (arg0->m_RotY + arg1);
    func_00210EF0(&arg0->m_RotY);
}
