#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 CPlayer__HasMenuOpen(CPlayer *arg0) {
    s32 var_v0;
    CCamera *temp_v1;

    temp_v1 = arg0->m_pCamera;
    var_v0 = 0;
    if (temp_v1 != NULL) {
        var_v0 = temp_v1->m_Menus.count != 0;
    }
    return var_v0;
}
