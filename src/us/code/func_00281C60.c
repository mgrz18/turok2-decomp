/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de_30.c, func_80239DE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
struct Triple;
struct Triple;
struct Channel;
extern float D_800A9C34;
struct Channel;
struct Channel {
    s32 mode;
    f32 level;
    s32 state[3];
};
struct Channel;
struct RecordRecAt18;
struct RecordRecAt18 {
    char pad[8];
    Triple triple;
    f32 value;
    struct Channel channels[3];
};
struct RecordRecAt18;
extern void func_00281C60(struct RecordRecAt18 *record, f32 first, f32 second, f32 third, f32 value, s32 mode, Triple triple);
void func_00281C60(struct RecordRecAt18 *record, f32 first, f32 second, f32 third, f32 value, s32 mode,
                   Triple triple) {
    record->triple = triple;
    record->value = value;
    record->channels[0].mode = mode;
    record->channels[1].mode = mode;
    record->channels[2].mode = mode;
    record->channels[0].level = first * D_800A9C34;
    record->channels[1].level = second * D_800A9C34;
    record->channels[2].level = third * D_800A9C34;
}
