/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00251448.c, func_802514A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern s32 func_00203068(s32 unused0, s32 unused1, s32 keep_recent);
struct Pool;
struct Pool {
    s32 count;
    char pad[0x40];
    s32 field44;
};
struct Pool;
typedef struct Pool Pool;
struct func_80251448_S1;
typedef struct func_80251448_S1 func_80251448_S1;
struct func_80251448_S1;
struct func_80251448_S1 {
    s32 unk0;
    char pad0[0x8 - 0x0 - sizeof(s32)];
    s32 unk8;
    char pad8[0xC - 0x8 - sizeof(s32)];
    s32 unkC;
    char padC[0x10 - 0xC - sizeof(s32)];
    s32 unk10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk14;
    char pad14[0x20 - 0x14 - sizeof(s32)];
    s32 unk20;
    char pad20[0x24 - 0x20 - sizeof(s32)];
    s32 unk24;
};
extern Pool D_800D8DAC;
extern void **D_800D81D4;
extern s32 CList__AddHead(void *arg0, s32 arg1);
static __inline__ void *pool_alloc(s32 arg1) {
    void *temp_s0;
    if (D_800D8DAC.count == 0) {
        return 0;
    }
    D_800D8DAC.count -= 1;
    temp_s0 = D_800D81D4[D_800D8DAC.count];
    ((func_80251448_S1 *)(temp_s0))->unkC = 0x800;
    ((func_80251448_S1 *)(temp_s0))->unk8 = 0;
    ((func_80251448_S1 *)(temp_s0))->unk0 = 0;
    ((func_80251448_S1 *)(temp_s0))->unk24 = 0;
    ((func_80251448_S1 *)(temp_s0))->unk20 = 0;
    ((func_80251448_S1 *)(temp_s0))->unk14 = 0;
    ((func_80251448_S1 *)(temp_s0))->unk10 = D_800D8DAC.field44;
    CList__AddHead((char *)&D_800D8DAC - 0xBCC, (s32)temp_s0);
    ((func_80251448_S1 *)(temp_s0))->unkC |= (arg1 & 0xC);
    return temp_s0;
}
void *func_002031E8(s32 unused0, s32 flags) {
    void *node;
    node = pool_alloc(flags);
    if (node == 0) {
        s32 keep = flags & 0x10;
        if (func_00203068(0, 2, keep == 0) != 0) {
            node = pool_alloc(flags);
        }
    }
    return node;
}
