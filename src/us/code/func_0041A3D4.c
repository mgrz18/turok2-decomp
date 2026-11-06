#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount;
extern char gEngineApp[];
extern void func_00430330(char *);

s32 ContinueMenu_No(s32 menu, Menu *c) {
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) {
        func_00430330(gEngineApp);
    } else {
        c->player->m_bActive = 0;
    }
    return 1;
}
