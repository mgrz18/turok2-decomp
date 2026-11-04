#include "common.h"
#include "structs/player.h"

void func_00256450(CPlayer *arg0) {
    CBarrel *temp_v0;

    temp_v0 = &arg0->m_Barrel;
    arg0->m_Barrel.m_State = 0;
    temp_v0->m_Turn = 0;
    temp_v0->m_nCursor = -1;
    temp_v0->m_Open = 0;
    arg0->m_bHeadMovementDisabled = 0;
}
