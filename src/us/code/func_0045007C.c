/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443120.c, func_00443120.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
struct O06A;
typedef struct { u8 pad[4]; s32 unk4; u8 pad2[0x10]; void *(*unk18)(struct O06A *); } O06D;
typedef struct O06A { u8 pad[0x14]; O06D *unk14; } O06A;
void func_0045007C(O06A *a, f32 v) {
    O06D *d = a->unk14;
    void *p = d->unk18(a);
    switch (d->unk4) {
    case 0: *(f32 *)p = v; break;
    case 3:
        if (p) {
            *(s8 *)p = (s32)v;
            break;
        } else {
            *(s8 *)p = (s32)v;
            break;
        }
    case 4: *(s16 *)p = (s32)v; break;
    case 1: case 2: case 5: *(s32 *)p = (s32)v; break;
    case 6: *(u8 *)p = (u32)v; break;
    case 7: *(u16 *)p = (u32)v; break;
    case 8: *(u32 *)p = (u32)v; break;
    }
}
