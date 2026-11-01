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
extern O01Text D_800B4BB8;
extern O01Text D_800B4BFC;
extern O01Text D_800B4C48;
extern O01Text D_800B4D1C;
extern O01Text D_800B4CB4;
extern O01Text D_800B4D84;
extern O01Text D_800B4DEC;
extern O01Text D_800B4E38;
extern O01Text D_800B4E70;
extern O01Text D_800B4E90;
extern O01Text D_800B4EF8;
extern O01Text D_800B4F1C;
extern O01Text D_800B4F68;
extern O01Text D_800B4F98;
extern O01Text D_800B4FD4;
extern O01Text D_800B5010;
s32 func_0045B7D0(O01Item *item) {
    switch (D_801309C0) {
    default:
    case 0:
        item->unk14 = &D_800B4BB8;
        break;
    case 1:
        item->unk14 = &D_800B4BFC;
        break;
    case 2:
        item->unk14 = &D_800B4C48;
        break;
    case 3:
        item->unk14 = &D_800B4D1C;
        break;
    case 4:
        item->unk14 = &D_800B4CB4;
        break;
    case 5:
        item->unk14 = &D_800B4D84;
        break;
    case 6:
        item->unk14 = &D_800B4DEC;
        break;
    case 7:
        item->unk14 = &D_800B4E38;
        break;
    case 8:
        item->unk14 = &D_800B4E70;
        break;
    case 9:
        item->unk14 = &D_800B4E90;
        break;
    case 10:
        item->unk14 = &D_800B4EF8;
        break;
    case 11:
        item->unk14 = &D_800B4F1C;
        break;
    case 12:
        item->unk14 = &D_800B4F68;
        break;
    case 13:
        item->unk14 = &D_800B4F98;
        break;
    case 14:
        item->unk14 = &D_800B4FD4;
        break;
    case 15:
        item->unk14 = &D_800B5010;
        break;
    }
    return 0;
}
