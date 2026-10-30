/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040A500.c, func_0040A500.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
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
typedef struct O01Cfg {
               u8 pad0[0xC];
               f32 unkC;
               f32 unk10;
               f32 unk14;
               u8 pad18[4];
               s32 unk1C;
               u8 pad20[2];
               s16 unk22;
               s32 unk24;
               u8 pad28[0x4E0];
               u32 unk508;     /* Rage Wars: 0x90 */
} O01Cfg;
extern O01Cfg *D_800C201C;
extern O01Text D_800B49DC;
extern O01Text D_800B49F0;
extern O01Text D_800B4A04;
s32 func_0045AD98(O01Item *item) {
    switch (D_800C201C->unk508) {
    default:
    case 0:
        item->unk14 = &D_800B49DC;
        break;
    case 16:
        item->unk14 = &D_800B49F0;
        break;
    case 32:
        item->unk14 = &D_800B4A04;
        break;
    }
    return 0;
}
