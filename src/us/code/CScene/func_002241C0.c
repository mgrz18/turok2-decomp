#include "common.h"
#include "structs/world.h"
#include "protos/scene.h"

u16 CScene__GetObjectTypeID(CScene *arg0, s32 arg1) {
    return arg0->m_pObjectTypes->ids[arg1];
}
