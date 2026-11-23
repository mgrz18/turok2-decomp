/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00217594.c, func_00217594.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
f32 Random_Blend(f32, f32);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define GameObject_DropPickupAt GameObject_DropPickupAt_i3
#endif
void GameObject_DropPickupAt(void *, s32, s32, s32, s32, f32);
typedef struct { u32 mask; s32 val; } B04Ent;
typedef struct { u8 pad0[4]; s32 x, y, z; } B04O; /* Turok 2: pos at 0x4 */
void AI_DropPickupsByMask(B04O *o, s32 a1, u32 mask, B04Ent *e) {
    for (; e->mask != 0; e++) {
        if (mask & e->mask) {
            GameObject_DropPickupAt(o, o->x, o->y, o->z, e->val, Random_Blend(4.5e2f, 6.0e2f));
        }
    }
}
