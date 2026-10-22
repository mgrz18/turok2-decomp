/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028BD88.CPP, func_8028BDAC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct Field_void_6C;
struct Field_void_6C {
    char pad[0x6C];
    void * value;
};
struct Field_void_6C;
typedef struct Field_void_6C Field_void_6C;
void *func_002017D4(void *table, s32 index);
s32 func_00224CF8(void *arg0, s32 arg1) {
    void *temp_v0;
    u8 *result;
    temp_v0 = func_002017D4((((struct Field_void_6C *) ((s8 *) arg0))->value), 1);
    func_002017D4(temp_v0, 0);
    result = (u8 *)((char *)func_002017D4(temp_v0, 1)) + arg1;
    return *result == 0;
}
