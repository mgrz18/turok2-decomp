/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004037E8.c, func_004037E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
extern s32 D_800C1BB8;
extern u8 *D_80119890;
void func_00412438(s32 a);
void func_0044BB28(s32 a0) {
    register s32 a asm("$7");
    register u8 *bits asm("$4");
    register s32 id asm("$5");
    u8 b;
    s32 m;
    s32 i;
    a = a0;
    bits = D_80119890;
    if (D_800C1BB8 != 0) {
        id = a - 0x8D;
        if ((u32)id < 6 && bits != 0) {
            bits += 0x9F4;
            i = id >> 3;
            m = 1 << (id & 7);
            b = bits[i];
            if ((b & m) != 0) return;
            bits[i] = b | m;
        }
        func_00412438(a);
    }
}
