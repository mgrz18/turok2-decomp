#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"

extern u8 gEngineApp_m_SwooshSystem[];
void SwooshSet_FreeOwner(void *list, void *actor);

void Action_GoEnter(u8 *actor) {
    s32 *table = &gScene;
    u8 *def = *(u8 **) (actor + 0x14);

    CScene__SetInstanceSavedBit(table, actor, 1);
    GameObject_FireTriggers(actor, 0x80000, actor);
    SwooshSet_FreeOwner(gEngineApp_m_SwooshSystem, actor);
    if (*table != 4) {
        *(s32 *) (actor + 0xD4) |= 0x8000000;
        if (*(s32 *) (def + 0x14) & 2) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s16 *) (*(u8 **) (actor + 0x14) + 0x28) == 0) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
    }
}
