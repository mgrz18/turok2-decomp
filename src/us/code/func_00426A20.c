/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_804053E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern int D_80130C60[];
struct Entry_func_804053E0_de;
struct Entry_func_804053E0_de {
    s32 size;
    char data[28];
};
struct Entry_func_804053E0_de;
struct RecordRecAt4V2;
struct RecordRecAt4V2 {
    s32 header;
    struct Entry_func_804053E0_de entries[16];
};
extern s32 func_00426A20(s32 index, s32 entry, s32 *out);
extern s32 func_00426E6C(s32);
s32 func_00426A20(s32 index, s32 entry, s32 *out) {
    extern struct RecordRecAt4V2 *D_800C2224;
    if (D_80130C60[index] != 3) {
        return -2;
    }
    *out = func_00426E6C(D_800C2224[index].entries[entry].size);
    return 0;
}
