#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "protos/scene.h"


s32 CScene__GetObject(CScene *arg0, s32 arg1) {
    s32 *table;

    table = arg0->m_pObjects;
    if (arg1 < *table) {
        return CISet__GetROMAddress((sptr)table, arg0->m_ObjectsKey, arg1);
    }
    return 0;
}
