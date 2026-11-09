#include "common.h"
#include "structs/player.h"

s32 CPlayerPool__GetFirstActive(CPlayerPool *arg0) {
    return (sptr) arg0->m_ActiveList.m_pHead;
}
