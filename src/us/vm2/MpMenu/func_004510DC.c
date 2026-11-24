#include "common.h"
#include "globals/menu_desc.h"

sptr MenuPool_OpenChildMenu_2(struct MenuPool *, struct Menu *, struct MenuDesc *);


s32 PlayerSetupMenu_Play_2(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) &gRumbleNoticeMenuDesc_2);
    return 1;
}
