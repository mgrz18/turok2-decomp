/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022AA34.c, func_0022AA34.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Actor Actor;
struct Actor { /* Turok 2 layout */
    u8 pad0[0xBE4];
    f32 unkBE4;         /* Rage Wars: 0x840 */
    u8 padBE8[4];
    s32 unkBEC;         /* Rage Wars: 0x848 */
};
extern f32 D_800A70E4[];
extern f32 D_800A70EC;
extern f32 D_800A70F0;
extern f32 D_800A70F4[];
f32 func_00253934(Actor *a) {
    f32 t = a->unkBE4 - D_800A70E4[1];
    register f32 tmp asm("$f0");
    register f32 v asm("$f1");
    if (a->unkBEC != 0) goto c0;
    tmp = t * D_800A70EC;
    __asm__("" : "+f"(tmp));
    v = tmp;
    if (D_800A70F0 < v) {
c0:
        v = D_800A70F4[0];
    }
    return D_800A70F4[1] - v;
}
