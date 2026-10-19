/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de.c, func_80279204_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Shared_World D_800F7078;
extern s32 func_00224B58(Shared_World *world, Trigger *trigger);
extern s32 func_00225210(Shared_World *world, s32 id);
extern void func_002252A0(Shared_World *world, s32 id);
extern void func_0026E294(Trigger *trigger);
extern s32 func_0026E6D4(Trigger *trigger, s32 arg);
extern void func_00224A68(Shared_World *world, Trigger *trigger, s32 arg);
extern void func_00224C98(Shared_World *world, s32 id);
extern s32 func_00224CF8(Shared_World *world, s32 id);
extern void func_0022515C(Shared_World *world, s32 id);
extern void func_0026E210(Trigger *trigger);
void func_0026E4B8(Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, s32 arg) {
    s32 i;
    s32 active;
    s32 usable;
    Shared_World *world;
    Trigger *trigger;
    for (trigger = triggers, i = 0; i < count; trigger++, i++) {
        world = &D_800F7078;
        usable = 1;
        if (world->mode != 4) {
            usable = func_00224B58(world, trigger) == 0;
        }
        if (!usable) {
            continue;
        }
        active = 1;
        if (!(trigger->flags.bytes.state & 0x20) && enter != 0 && (trigger->mask & enter)) {
            trigger->flags.bytes.state |= 0x40;
            if (trigger->flags.bytes.id != 0) {
                if (func_00225210(world, trigger->flags.bytes.id) != 0) {
                    func_002252A0(world, trigger->flags.bytes.id);
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
        if ((trigger->flags.word & 0x1100) == 0x1000) {
            active = 0;
        }
        if (func_0026E6D4(trigger, arg) == 0) {
            active = 0;
        }
        if (!active) {
            continue;
        }
        if (trigger->flags.bytes.id != 0) {
            if (!(trigger->flags.bytes.state & 0x80)) {
                func_00224A68(world, trigger, 1);
                trigger->flags.bytes.state |= 0x80;
                func_00224C98(world, trigger->flags.bytes.id);
                if (func_00224CF8(world, trigger->flags.bytes.id) != 0) {
                    func_0022515C(world, trigger->flags.bytes.id);
                }
            }
        } else {
            func_0026E210(trigger);
        }
    }
}
