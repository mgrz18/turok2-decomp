/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284980.c, func_00284980.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef struct Obj {
    s8 kind;
    u8 pad1;
    u16 id; /* 0x2 in Turok 2 (0x4 in Rage Wars) */
} Obj;

int func_00232FBC(Obj *o) {
    switch (o->id) {
    case 8:
    case 0xD:
    case 0x12:
        return 1;
    default:
        return 0;
    }
}
