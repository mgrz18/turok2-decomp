#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"


s32 func_00421250();                            /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Unk00419618;

#ifdef TARGET_PORT
/* The port: the view read the player and the controller at the N64 offsets 0x1C/0x20; on the host they
 * are the typed Menu's (structs/menu.h), pointer-wide. The load menu got the low half of the window's
 * desc pointer as its controller (its buttons garbage: the cursor ran up every frame, A never confirmed). */
#include "structs/menu.h"
s32 SingleMenu_Load(s32 arg0, Menu *arg1) {
    func_00421250();
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, (sptr) arg1->player, (sptr) arg1->controller, 0);
    return 1;
}
#else
s32 SingleMenu_Load(s32 arg0, Unk00419618 *arg1) {
    func_00421250();
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, arg1->unk1C, arg1->unk20, 0);
    return 1;
}
#endif
