/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00260CBC.c, func_00260CBC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct { u32 bits; f32 min; f32 range; } P3QInfo;
typedef struct { u32 pos; P3QInfo q; } P3QArr;
f32 func_0021813C(P3QArr *a, s32 idx) {
    P3QArr loc; u32 pos; u32 *w; u32 sh; u32 v;
    u32 hi; u32 bits; register f32 fa asm("$f0"); f32 fb; pos = a->pos + a->q.bits * idx; loc.q = a->q; bits = loc.q.bits; w = (u32 *)((pos & 0xF0000000) | ((pos & 0x0FFFFFE0) >> 3)); sh = pos & 0x1F; v = w[0]; hi = w[1]; if (sh != 0) { v >>= sh; hi <<= 32 - sh; v |= hi; } if (bits < 32) { v &= (1 << bits) - 1; } fa = (f32)v; fb = (f32)((1 << bits) - 1); return fa / fb * loc.q.range + loc.q.min;
}
