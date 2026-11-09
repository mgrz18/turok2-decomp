/* t2modelog.c: engine state for the app's log (port/app/game.cpp prints every change): the
 * engine mode and, with T2_MENU_LOG, the newest open menu (its desc address; resolve it with nm)
 * and its cursor. Bring-up aid for headless runs. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/engine.h"

int T2Game_EngineMode(void) {
    return gEngineApp.m_Settings.mode;
}

/* the last menu opened in the game's menu pool (NULL: none) and its cursor */
const void *T2Game_TopMenu(int *cursor) {
    MenuPool *pool = gEngineApp.m_Game.m_MenuPool;
    Menu *m = pool->tail;
    *cursor = m != NULL ? m->cursor : -1;
    return m != NULL ? (const void *)m->desc : NULL;
}
