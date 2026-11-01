/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040ACD4.c, func_0040ACD4.
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
extern s32 D_801309C0;
extern O01Text D_800B4BB0;
extern O01Text D_800B4BB4;
s32 func_0045BDAC(O01Item *item) {
    switch (D_801309C0) {
    default:
    case 0:
        item->unk14 = &D_800B4BB0;
        break;
    case 5:
    case 6:
    case 9:
    case 10:
    case 12:
    case 15:
        item->unk14 = &D_800B4BB4;
        break;
    }
    return 0;
}
