/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204620.c, func_00204620.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct B01Def {
    char pad0[0x28];
    s16 unk28;
} B01Def;
/* Turok 2: def at 0x14, flags at 0xD4 (Rage Wars 0x18 / 0x100). */
typedef struct Actor {
    char pad0[0x14];
    B01Def *def;
    char pad18[0xBC];
    u32 flags;
} Actor;
s32 func_00266284(Actor *actor) {
    s32 v = actor->def->unk28;
    if (v != 0) {
        actor->flags |= 0x2000;
    } else {
        actor->flags &= ~0x2000;
        actor->flags &= ~0x100;
    }
    return v;
}
