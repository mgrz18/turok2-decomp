/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A6DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct RecordRecAt24;
struct RecordRecAt24 {
    char pad[0x1C];
    s32 first;
    s32 second;
    void *resource;
};
struct RecordRecAt24;
extern s32 func_00422124(void *unused, struct RecordRecAt24 *record);
extern s32 D_801309B8;
extern char D_80119E2C[];
extern char D_0043B6F0[];
extern void func_00416644(void *, void *, s32, s32, s32);
s32 func_00422124(void *unused, struct RecordRecAt24 *record) {
    do {
        if (D_801309B8 != 0) {
            func_00416644(D_80119E2C, D_0043B6F0, record->first, record->second, 0);
        } else {
            func_00416644(D_80119E2C, record->resource, record->first, record->second, 0);
        }
    } while (0);
    return 1;
}
