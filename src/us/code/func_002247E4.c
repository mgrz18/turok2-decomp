/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B874.c, func_0028B874.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef struct { u8 pad0[0x16E]; u16 f16E; } P4EntB;
typedef struct { u8 pad0[0x6C]; s32 f6C; } P4OwnB;
s32 func_002017D4(s32 a, s32 b);
void func_00201818(s32 a, s32 b);
void func_002247E4(P4OwnB *o, P4EntB *e, s32 set) {
    s32 bit = e->f16E;
    s32 idx = *(s32 *)((u8 *)o + 0x18FA8);
    s32 n = func_002017D4(func_002017D4(func_002017D4(o->f6C, 0), idx), 1);
    func_002017D4(n, 0);
    func_00201818(n, 1);
    {
        u8 *bits = (u8 *)func_002017D4(n, 1);
        u8 *p2 = bits;
        s32 m = 1 << (bit & 7);
        if (set != 0) {
            bits[bit / 8] |= m;
        } else {
            p2[bit / 8] &= ~m;
        }
    }
}
