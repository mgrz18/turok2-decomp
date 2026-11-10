#include "common.h"
#include "m2c_macros.h"

#ifdef TARGET_PORT
#include "structs/particle.h"
CDLight *CDLightSet__GetFirst(CDLightSet *arg0) {
    return arg0->m_UsedList.m_pHead;
}
#else
s32 CDLightSet__GetFirst(s32 *arg0) {
    return *arg0;
}
#endif
