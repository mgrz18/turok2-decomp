/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002AD940.c, func_002AD940.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Row14 {
    s32 pad0;
    s16 id;
    u8 pad6[14];
} Row14;
extern Row14 D_800AD498[];
Row14 *func_0023EF64(s32 id) {
    s32 i;
    Row14 *p = D_800AD498;
    for (i = 0x23; i != -1; i--) {
        if (p->id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}
