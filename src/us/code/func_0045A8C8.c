/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_5.CPP, func_80409EF4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
struct PakMenuSlot;
struct RecordRecAt20V2;
struct PakMenuSlot;
struct RecordRecAt20V2 {
    char pad[0x1C];
    s32 result;
    struct PakMenuSlot *inner;
};
struct RecordRecAt20V2;
struct PakMenuSlot;
extern s32 D_800C2028;
extern s32 func_0045A8C8(struct RecordRecAt20V2 *record, s32 *out);
extern s32 D_80130990;
s32 func_0045A8C8(struct RecordRecAt20V2 *record, s32 *out) {
    s32 value;
    s32 result;
    if (D_80130990 != 0) {
        value = D_800C2028;
        result = 0;
    } else {
        value = record->inner->unk4;
        result = record->result;
    }
    *out = value;
    return result;
}
extern s32 D_80130990;
