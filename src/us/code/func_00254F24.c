#include "common.h"
#include "globals/engine.h"
#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/weapon.h"
#include "structs/controller.h"
#endif

#ifdef TARGET_PORT
/* The host body on the members (soak S6): the wheel's m_bStickWheel/m_nCursor at N64 0x380/0x37C
 * and the player's controller at N64 0xA50 (its buttons at 0xB0). CBarrelItem holds a pointer, so
 * the host offsets differ: the stick wheel read m_Items and crashed on a garbage controller. */
s32 CBarrel__ShouldClose(u8 *self, u8 *other) {
    CBarrel *menu = (CBarrel *) self;
    CPlayer *player = (CPlayer *) other;
    if (menu->m_bStickWheel != 0) {
        if (player->m_ContState.m_pContState->buttons & 0xC000) {
            return 0;
        }
        menu->m_nCursor = -1;
        return 1;
    }
    menu->m_Turn -= gFrameIncrement;
    return menu->m_Turn <= 0.0f;
}
#else
s32 CBarrel__ShouldClose(u8 *self, u8 *other) {
    if (*(s32 *)(self + 0x380) != 0) {
        if (*(s32 *)(*(u8 **)(other + 0xA50) + 0xB0) & 0xC000) {
            return 0;
        }
        *(s32 *)(self + 0x37C) = -1;
        return 1;
    }
    *(f32 *)(self + 4) -= gFrameIncrement;
    return *(f32 *)(self + 4) <= 0.0f;
}
#endif
