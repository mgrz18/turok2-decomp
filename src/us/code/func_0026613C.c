#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the type id (N64 0xB8) and the flags (0xD4) by member of CGameObjectInstance. */
void Action_Advance(u8 *actor_, u8 *state) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;

    if (CScene__GetInstanceSavedBit(&gScene, actor) == 1) {
        CAI__SetMode(actor, state, 1);
        switch ((u16) actor->m_nTypeFlag) {
            case 0x64C:
                actor->m_dwFlags &= ~0x2000;
                actor->m_dwFlags &= ~0x100;
                break;
            case 0x64B:
            case 0x64E:
                actor->m_dwFlags &= ~0x8000000;
                break;
        }
    } else {
        CAI__SetMode(actor, state, 0);
    }
}
#else
void Action_Advance(u8 *actor, u8 *state) {
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 1) {
        CAI__SetMode(actor, state, 1);
        switch (*(u16 *) (actor + 0xB8)) {
            case 0x64C:
                *(s32 *) (actor + 0xD4) &= ~0x2000;
                *(s32 *) (actor + 0xD4) &= ~0x100;
                break;
            case 0x64B:
            case 0x64E:
                *(s32 *) (actor + 0xD4) &= ~0x8000000;
                break;
        }
    } else {
        CAI__SetMode(actor, state, 0);
    }
}
#endif
