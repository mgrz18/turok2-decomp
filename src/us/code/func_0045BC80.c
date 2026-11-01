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
extern O01Text D_800B4BF8;
extern O01Text D_800B4C3C;
extern O01Text D_800B4CA8;
extern O01Text D_800B4D7C;
extern O01Text D_800B4D14;
extern O01Text D_800B4DE0;
extern O01Text D_800B4E34;
extern O01Text D_800B4E6C;
extern O01Text D_800B4E8C;
extern O01Text D_800B4EEC;
extern O01Text D_800B4F18;
extern O01Text D_800B4F64;
extern O01Text D_800B4F94;
extern O01Text D_800B4FD0;
extern O01Text D_800B500C;
extern O01Text D_800B506C;
s32 func_0045BC80(O01Item *item) {
    switch (D_801309C0) {
    default:
    case 0:
        item->unk14 = &D_800B4BF8;
        break;
    case 1:
        item->unk14 = &D_800B4C3C;
        break;
    case 2:
        item->unk14 = &D_800B4CA8;
        break;
    case 3:
        item->unk14 = &D_800B4D7C;
        break;
    case 4:
        item->unk14 = &D_800B4D14;
        break;
    case 5:
        item->unk14 = &D_800B4DE0;
        break;
    case 6:
        item->unk14 = &D_800B4E34;
        break;
    case 7:
        item->unk14 = &D_800B4E6C;
        break;
    case 8:
        item->unk14 = &D_800B4E8C;
        break;
    case 9:
        item->unk14 = &D_800B4EEC;
        break;
    case 10:
        item->unk14 = &D_800B4F18;
        break;
    case 11:
        item->unk14 = &D_800B4F64;
        break;
    case 12:
        item->unk14 = &D_800B4F94;
        break;
    case 13:
        item->unk14 = &D_800B4FD0;
        break;
    case 14:
        item->unk14 = &D_800B500C;
        break;
    case 15:
        item->unk14 = &D_800B506C;
        break;
    }
    return 0;
}
