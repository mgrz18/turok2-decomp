#include "common.h"
#include "globals/engine.h"
#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/weapon.h"
#include "structs/controller.h"
#endif

#ifdef TARGET_PORT
/* The host body by member (round 9 x4): the N64 view (0x380 m_bStickWheel, 0x37C m_nCursor,
 * 4 m_Turn of the CBarrel; 0xA50 m_ContState.m_pContState of the CPlayer) read other host
 * fields, so the open wheel closed on its first frame while A was still held: the head
 * movement came back on and the demo's stick turned the player instead of choosing a weapon
 * (attract demo 1 walked into the wall beside its door). The same fix came from the soak (x5
 * S6): with the stick wheel the N64 offsets read m_Items and crashed on a garbage controller.
 * One body, typed like its caller (func_0024BD18.c). */
s32 CBarrel__ShouldClose(CBarrel *self, CPlayer *other) {
    if (self->m_bStickWheel != 0) {
        if (other->m_ContState.m_pContState->buttons & 0xC000) {
            return 0;
        }
        self->m_nCursor = -1;
        return 1;
    }
    self->m_Turn -= gFrameIncrement;
    return self->m_Turn <= 0.0f;
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
