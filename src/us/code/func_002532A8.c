#include "common.h"
#include "structs/player.h"

s32 CPlayerPool__GetFirstActive(CPlayerList *arg0) {
    return (s32) arg0->m_ActiveList.m_pHead;
}
