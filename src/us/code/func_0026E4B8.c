/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de.c, func_80279204_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "protos/scene.h"

extern void func_0026E294(Trigger *trigger);
extern s32 LevelEvent_CheckRequiredItem(Trigger *trigger, sptr arg);
extern void LevelEvent_Trigger(Trigger *trigger);
void LevelEvent_ProcessTriggers(Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, sptr arg) {
    s32 i;
    s32 active;
    s32 usable;
    CScene *world;
    Trigger *trigger;
    for (trigger = triggers, i = 0; i < count; trigger++, i++) {
        world = &gScene;
        usable = 1;
        if (world->m_Mode != 4) {
            usable = CScene__IsEventFired(world, trigger) == 0;
        }
        if (!usable) {
            continue;
        }
        active = 1;
        if (!(trigger->flags.bytes.state & 0x20) && enter != 0 && (trigger->mask & enter)) {
            trigger->flags.bytes.state |= 0x40;
            if (trigger->flags.bytes.id != 0) {
                if (CScene__AreEventsDone(world, trigger->flags.bytes.id) != 0) {
                    CScene__FireEventsByKeyOnce(world, trigger->flags.bytes.id);
                }
            } else {
                func_0026E294(trigger);
            }
            active = 1;
        }
        if (require != 0) {
            active = 0;
            if (trigger->mask & require) {
                active = 1;
            }
        }
        if (enter != 0 && !(trigger->mask & enter)) {
            active = 0;
        }
        if ((trigger->mask & group) && !(trigger->mask & group & exclude)) {
            active = 0;
        }
#ifdef TARGET_PORT /* the big-endian word's 0x1100 bits are the state byte's 0x11 (byte 0xE) */
        if ((trigger->flags.bytes.state & 0x11) == 0x10) {
#else
        if ((trigger->flags.word & 0x1100) == 0x1000) {
#endif
            active = 0;
        }
        if (LevelEvent_CheckRequiredItem(trigger, arg) == 0) {
            active = 0;
        }
        if (!active) {
            continue;
        }
        if (trigger->flags.bytes.id != 0) {
            if (!(trigger->flags.bytes.state & 0x80)) {
                CScene__SetEventFired(world, trigger, 1);
                trigger->flags.bytes.state |= 0x80;
                CScene__DecLinkGroupCount(world, trigger->flags.bytes.id);
                if (CScene__IsLinkGroupDone(world, trigger->flags.bytes.id) != 0) {
                    CScene__FireEventsByKey(world, trigger->flags.bytes.id);
                }
            }
        } else {
            LevelEvent_Trigger(trigger);
        }
    }
}
