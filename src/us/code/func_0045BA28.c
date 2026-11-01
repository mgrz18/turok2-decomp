/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
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
extern O01Text D_800B4BE8;
extern O01Text D_800B4C18;
extern O01Text D_800B4C78;
extern O01Text D_800B4D4C;
extern O01Text D_800B4CE4;
extern O01Text D_800B4DB0;
extern O01Text D_800B4E14;
extern O01Text D_800B4E58;
extern O01Text D_800B4E78;
extern O01Text D_800B4EBC;
extern O01Text D_800B4F10;
extern O01Text D_800B4F48;
extern O01Text D_800B4F84;
extern O01Text D_800B4FBC;
extern O01Text D_800B4FF8;
extern O01Text D_800B503C;
s32 func_0045BA28(O01Item *item) {
    switch (D_801309C0) {
    default:
    case 0:
        item->unk14 = &D_800B4BE8;
        break;
    case 1:
        item->unk14 = &D_800B4C18;
        break;
    case 2:
        item->unk14 = &D_800B4C78;
        break;
    case 3:
        item->unk14 = &D_800B4D4C;
        break;
    case 4:
        item->unk14 = &D_800B4CE4;
        break;
    case 5:
        item->unk14 = &D_800B4DB0;
        break;
    case 6:
        item->unk14 = &D_800B4E14;
        break;
    case 7:
        item->unk14 = &D_800B4E58;
        break;
    case 8:
        item->unk14 = &D_800B4E78;
        break;
    case 9:
        item->unk14 = &D_800B4EBC;
        break;
    case 10:
        item->unk14 = &D_800B4F10;
        break;
    case 11:
        item->unk14 = &D_800B4F48;
        break;
    case 12:
        item->unk14 = &D_800B4F84;
        break;
    case 13:
        item->unk14 = &D_800B4FBC;
        break;
    case 14:
        item->unk14 = &D_800B4FF8;
        break;
    case 15:
        item->unk14 = &D_800B503C;
        break;
    }
    return 0;
}
