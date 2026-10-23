/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80204E78_4.CPP, func_80205494_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;

typedef struct Descriptor {
    char pad0[0x14];
    s32 damageFlags;            /* 0x14 */
} Descriptor;

typedef struct BreakableHitContext {
    char pad0[0x14];
    Descriptor *def;            /* 0x14 (Rage Wars 0x18) */
    char pad18[0xD4 - 0x18];
    s32 flags;                  /* 0xD4 (Rage Wars 0x100) */
} BreakableHitContext;

typedef struct ObjectStateCC {
    char pad0[0xC7];
    s8 unk_C7;                  /* 0xC7 (Rage Wars 0xCB) */
} ObjectStateCC;

void func_00265CC8(BreakableHitContext *arg0, ObjectStateCC *arg1) {
    Descriptor *def = arg0->def;
    if (arg1->unk_C7 != 0 && (def->damageFlags & 0x4) != 0) {
        unsigned int flags = arg0->flags;
        flags &= ~0x2000;
        flags &= ~0x100;
        arg0->flags = flags;
    }
}
