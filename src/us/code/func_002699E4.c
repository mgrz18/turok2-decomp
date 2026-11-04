#include "structs/game_object.h"

s32 func_00268E60(CGameObjectInstance *obj);
void func_00275544(s32 sound, CVector3 pos, s32 arg4, s32 arg5);
void func_00225C08(void *table, s32 arg1, CVector3 *pos);
extern u8 D_800F7078[];

void func_002699E4(CGameObjectInstance *obj) {
    s32 kind = func_00268E60(obj);
    s32 sound;

    if (kind == -1) {
        return;
    }
    switch (kind) {
        default:
        case 0:
        case 4:
        case 9:
            sound = 0x39F;
            break;
        case 1:
        case 5:
        case 6:
        case 7:
        case 8:
            sound = 0x3A1;
            break;
        case 2:
            sound = 0x39E;
            break;
        case 3:
            sound = 0x39D;
            break;
        case 10:
            sound = 0x3A0;
            break;
    }
    func_00275544(sound, obj->header.m_vPos, 0, -1);
    func_00225C08(D_800F7078, 1, &obj->header.m_vPos);
}
