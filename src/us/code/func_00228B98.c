#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT
#include "structs/particle.h"
CDLight *CDLightSet__GetNext(CDLightSet *arg0, CDLight *arg1) {
    return arg1->m_pNext;
}
#else
s32 CDLightSet__GetNext(sptr arg0, CAI *arg1) {
    return arg1->m_Health;
}
#endif
