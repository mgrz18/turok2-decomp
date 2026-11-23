#include "common.h"
#include "structs/game_object.h"

#include "structs/particle.h"
CDLight *CDLightSet__GetNext(CDLightSet *arg0, CDLight *arg1) {
    return arg1->m_pNext;
}
