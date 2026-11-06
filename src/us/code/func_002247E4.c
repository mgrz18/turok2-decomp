/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B874.c, func_0028B874.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef struct { u8 pad0[0x16E]; u16 f16E; } P4EntB;
typedef struct { u8 pad0[0x6C]; s32 f6C; } P4OwnB;
s32 CISet__GetBlock(s32 a, s32 b);
void CISet__GetBlockSize(s32 a, s32 b);
void CScene__SetLevelActorKilled(P4OwnB *o, P4EntB *e, s32 set) {
    s32 bit = e->f16E;
    s32 idx = *(s32 *)((u8 *)o + 0x18FA8);
    s32 n = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(o->f6C, 0), idx), 1);
    CISet__GetBlock(n, 0);
    CISet__GetBlockSize(n, 1);
    {
        u8 *bits = (u8 *)CISet__GetBlock(n, 1);
        u8 *p2 = bits;
        s32 m = 1 << (bit & 7);
        if (set != 0) {
            bits[bit / 8] |= m;
        } else {
            p2[bit / 8] &= ~m;
        }
    }
}
