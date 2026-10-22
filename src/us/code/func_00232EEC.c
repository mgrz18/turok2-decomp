/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002848B0.c, func_002848B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct B19Obj {
    u16 unk0;
    u16 id; /* 0x2 */
} B19Obj;
s32 func_00232EEC(B19Obj *o) {
    switch (o->id) {
    case 2:
    case 0x56:
    case 0x111:
    case 0x126:
        return 1;
    default:
        return 0;
    }
}
