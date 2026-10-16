/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A77C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct Target_func_8040A77C_de;
struct Target_func_8040A77C_de {
    char pad0[0xA8];
    s32 first;
    char padAC[0xD0 - 0xAC];
    s32 second;
};
struct RecordRecAtCV1;
struct Target_func_8040A77C_de;
struct RecordRecAtCV1 {
    char pad[0xC];
    struct Target_func_8040A77C_de *target;
};
struct RecordRecAtCV1;
extern void func_004221C4(struct RecordRecAtCV1 *record);
extern int D_80130960;
extern s32 D_801309A8;
extern s32 D_80130960;
void func_004221C4(struct RecordRecAtCV1 *record) {
    record->target->first &= ~0x01800000;
    D_80130960 = 0;
    D_801309A8 = 1;
    record->target->second &= ~0x01800000;
}
