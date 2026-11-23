#include "common.h"
#include "structs/menu.h"
#include "rw/abu_protos.h"
#include "globals/game.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ s32 flags;
} Target;


void StatsMenu_ShowTagsRow_2(Menu *arg0) {
    Target *temp_a0;
    Target *temp_v0;

    if (gEngineApp_m_Settings.match.tag != 0) {
        temp_v0 = (Target *) arg0->items;
        temp_v0->flags = (s32) (temp_v0->flags | 0x01000000);
        return;
    }
    temp_a0 = (Target *) arg0->items;
    temp_a0->flags = (s32) (temp_a0->flags & 0xFEFFFFFF);
}
