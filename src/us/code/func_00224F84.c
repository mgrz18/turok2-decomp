#include "common.h"
#include "structs/world.h"

extern s32 func_00201848(s32, s32, s32);

s32 func_00224F84(CWorld *arg0, s32 arg1) {
    s32 *table;

    table = arg0->m_pObjects;
    if (arg1 < *table) {
        return func_00201848((s32)table, arg0->m_ObjectsKey, arg1);
    }
    return 0;
}
