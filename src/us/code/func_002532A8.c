#include "common.h"
#include "structs/player.h"

sptr CPlayerPool__GetFirstActive(CPlayerPool *arg0) {
    return (sptr) arg0->m_ActiveList.m_pHead;
}
