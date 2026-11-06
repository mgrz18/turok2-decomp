#include "common.h"
#include "globals/scene.h"

s32 CScene__GetInstanceSavedBit(void *table, void *actor);
s32 CAI__SetMode(void *actor, void *state, s32 mode);

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
