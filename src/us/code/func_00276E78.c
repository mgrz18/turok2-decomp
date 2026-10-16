/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_8.c, func_8025D238_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct ContextRecAt2BB8;
typedef struct ContextRecAt2BB8 ContextRecAt2BB8;
extern s32 func_00276E78(void **arg0, s16 id, s16 avoid);
struct ContextRecAt2BB8;
struct ContextRecAt2BB8 {
    char pad[0x2B50];
    s32 unk2B50;
    char p54[12];
    s32 unk2B60;
    s32 unk2B64;
    char p68[0x3C];
    f32 unk2BA4;
    char pa8[16];
    s32 unk2BB8;
};
struct ContextRecAt2BB8;
extern s32 func_00266D8C(s32, s32, s32, s32 *, s32 *);
extern s32 func_002113A4(void);
s32 func_00276E78(void **arg0, s16 id, s16 avoid) {
    s32 first;
    s32 last;
    s32 total;
    s32 entry;
    s32 draw;
    ContextRecAt2BB8 *table;
    s32 result;
    total = 0;
    table = *arg0;
    if (func_00266D8C(table->unk2B60, table->unk2B64, id, &first, &last) == 0) {
        return -1;
    }
    result = first;
    if (result != last) {
        for (entry = first; entry <= last; entry++) {
            total += 100;
        }
        draw = func_002113A4() % total;
        total = 0;
        for (entry = first; entry < last; entry++) {
            total += 100;
            if (total >= draw) {
                break;
            }
        }
        if (avoid != 0 && entry == avoid) {
            if (entry == last) {
                entry--;
            } else {
                entry++;
            }
        }
        result = entry;
    }
    return result;
}
