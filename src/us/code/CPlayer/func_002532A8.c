#include "common.h"
#include "structs/player.h"

sptr CPlayerPool__GetFirstActive(void *pool) {
    return (sptr) ((CPlayerPool *) pool)->m_ActiveList.m_pHead;
}
