#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"


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
