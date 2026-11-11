#include "common.h"
#include "globals/game.h"

extern f32 D_800C1064;

/* Plain pointer arithmetic, not struct fields: GCC marks struct accesses
 * MEM_IN_STRUCT and would then hoist them above the global store. */
#define OBJ(arg)        (*(u8 **) ((u8 *) (arg) + 0xC))
#define OBJ_FLAGS(obj)  (*(s32 *) ((u8 *) (obj) + 0x30))

#ifdef TARGET_PORT
#include "structs/menu.h"

/* The port: by member. The N64 reads the window's items at 0xC and the word at items + 0x30: the
 * second item's flags (MenuItem is 0x28 bytes on the N64, larger on the host). The game over /
 * continue window crashed here when the last life was lost. */
void GameOverMenu_Init(void *arg0) {
    gEngineApp_m_Settings.match.countdown = D_800C1064;
    ((Menu *) arg0)->items[1].flags &= ~0x04000000;
}
#else
void GameOverMenu_Init(void *arg0) {
    u8 *obj;

    gEngineApp_m_Settings.match.countdown = D_800C1064;
    obj = OBJ(arg0);
    OBJ_FLAGS(obj) &= ~0x04000000;
}
#endif
