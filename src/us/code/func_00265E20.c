/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80204E78_5.CPP, func_802055EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
typedef struct BreakableDefinition {
    char reserved0[0x14];
    s32 flags;
} BreakableDefinition;
/* Turok 2: def at 0x14, flags at 0xD4 (Rage Wars 0x18 / 0x100). */
typedef struct LevelActor {
    char pad0[0x14];
    BreakableDefinition *definition;
    char pad18[0xBC];
    s32 flags;
} LevelActor;
/* Turok 2: this state byte is at 0xC7 (Rage Wars 0xCB). */
typedef struct ObjectStateC7 {
    char pad0[0xC7];
    s8 unk_C7;
} ObjectStateC7;
void func_00265E20(void *arg0, void *arg1) {
    LevelActor *object = arg0;
    BreakableDefinition *nested = object->definition;
    ObjectStateC7 *state = arg1;
    if (state->unk_C7 != 0 &&
        (nested->flags & 0x20) != 0) {
        unsigned int flags = object->flags;
        flags &= ~0x2000;
        flags &= ~0x100;
        object->flags = flags;
    }
}
