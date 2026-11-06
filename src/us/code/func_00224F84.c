#include "common.h"
#include "structs/world.h"

extern s32 CISet__GetROMAddress(s32, s32, s32);

s32 CScene__GetObject(CScene *arg0, s32 arg1) {
    s32 *table;

    table = arg0->m_pObjects;
    if (arg1 < *table) {
        return CISet__GetROMAddress((s32)table, arg0->m_ObjectsKey, arg1);
    }
    return 0;
}
