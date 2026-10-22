/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002AD90C.c, func_002AD90C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Row18 {
    s32 pad0;
    s16 id;
    u8 pad6[18];
} Row18;
extern Row18 D_800AD768[];
Row18 *func_0023EF98(s32 id) {
    s32 i;
    Row18 *p = D_800AD768;
    for (i = 6; i != -1; i--) {
        if (p->id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}
