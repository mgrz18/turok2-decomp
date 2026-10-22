/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024DEF8.c, func_0024DEF8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct ObjDef {
    s32 type;
    u8 pad4[0x44];
    u32 flags48;        /* 0x48 */
} ObjDef;
/* Turok 2 actor: def at 0x14, health at 0x144 (Rage Wars: 0x18 / 0x174). */
typedef struct Obj {
    u8 pad0[0x14];
    ObjDef *def;        /* 0x014 */
    u8 pad18[0x12C];
    s32 unk144;         /* 0x144 */
} Obj;
s32 func_00267F10(Obj *a) {
    ObjDef *d = a->def;
    s32 r;
    switch (d->type) {
    case 11:
        return 1;
    case 1:
        r = 0;
        if (a->unk144 <= 0 || (d->flags48 & 2)) {
            r = 1;
        }
        return r;
    default:
        return 0;
    }
}
