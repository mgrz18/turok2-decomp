/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_15.c, func_80278E7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"

extern s32 CScene__QueueDelayedEvent(struct WorldRecAt1B40C *arg0, Record14 *arg1);
extern CScene gScene;
extern s32 CScene__SetEventFired(void *arg0, void *arg1, s32 arg2);
void LevelEvent_Trigger(void *arg0) {
    extern s32 LevelEvent_Fire(void *arg0);
    func_80278EEC_S1 *object = arg0;
    object->unkE |= 0x10;
    CScene__SetEventFired(&gScene, arg0, 1);
    if ((gScene.m_Mode == 4) && (object->unkE & 2)) {
        CScene__QueueDelayedEvent(&gScene, arg0);
        return;
    }
    LevelEvent_Fire(arg0);
}
