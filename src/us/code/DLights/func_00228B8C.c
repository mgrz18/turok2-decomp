#include "common.h"
#include "m2c_macros.h"

#include "structs/particle.h"
CDLight *CDLightSet__GetFirst(CDLightSet *arg0) {
    return arg0->m_UsedList.m_pHead;
}
