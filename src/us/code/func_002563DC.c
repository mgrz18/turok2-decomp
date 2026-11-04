#include "common.h"
#include "structs/player.h"

void func_002563DC(CPlayer *arg0) {
    CBarrel *temp_v0;

    temp_v0 = &arg0->m_Barrel;
    arg0->m_Barrel.m_nItems = 0;
    temp_v0->unk_0x4 = 0;
    temp_v0->m_nLastItem = -1;
    temp_v0->unk_0x8 = 0;
    arg0->m_bHeadMovementDisabled = 0;
}
