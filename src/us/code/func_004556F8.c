/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030.CPP, func_80444C68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct RecordRecAt14;
struct RecordRecAt14 {
    char pad[8];
    s32 flags;
    char padC[0x14 - 0xC];
    s32 value;
};
extern f32 D_80130930;
extern f32 D_800B6D28;
extern s32 func_00266B80(void);
extern s32 func_00424500();
s32 func_004556F8(struct RecordRecAt14 *record) {
    D_80130930 -= D_800B6D28;
    if (D_80130930 <= 0.0f) {
        D_80130930 = 0.0f;
    }
    if (func_00266B80() != 0x400000) {
        record->flags |= 0x1000000;
    } else {
        record->flags &= ~0x1000000;
    }
    record->value = func_00424500();
    return 0;
}
