#include "common.h"
#include "structs/player.h"

void CBarrel__Close(CBarrel *arg0) {
    arg0->m_State = 0;
    arg0->m_Turn = 0;
    arg0->m_nCursor = -1;
    arg0->m_Open = 0;
}
