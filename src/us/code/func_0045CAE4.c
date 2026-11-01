/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct O01Text {
              char *buf;
} O01Text;
typedef struct O01Flags {
               u8 pad0[0xA8];
               u32 unkA8;
               u8 padAC[0x24];
               u32 unkD0;
               u8 padD4[0x4C];
                u32 unk120;
} O01Flags;
typedef struct O01Item {
               u8 pad0[0xC];
               O01Flags *unkC;
               u8 pad10[4];
               O01Text *unk14;
} O01Item;
extern s32 D_801309B0;
extern O01Text D_800B5154;
extern O01Text D_800B51B0;
extern O01Text D_800B51EC;
extern O01Text D_800B5240;
extern O01Text D_800B529C;
s32 func_0045CAE4(O01Item *item) {
    switch (D_801309B0) {
    default:
    case 0:
        item->unk14 = &D_800B5154;
        break;
    case 1:
        item->unk14 = &D_800B51B0;
        break;
    case 2:
        item->unk14 = &D_800B51EC;
        break;
    case 3:
        item->unk14 = &D_800B5240;
        break;
    case 4:
        item->unk14 = &D_800B529C;
        break;
    }
    return 0;
}
