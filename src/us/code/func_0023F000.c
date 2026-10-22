/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002AD9DC.c, func_002AD9DC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Row10 {
    s32 pad0;
    s16 id;
    u8 pad6[10];
} Row10;
extern Row10 D_800AD838[];
Row10 *func_0023F000(s32 id) {
    s32 i;
    Row10 *p = D_800AD838;
    for (i = 14; i != -1; i--) {
        if (p->id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}
