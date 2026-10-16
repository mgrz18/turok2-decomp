/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_8025DB80_de.c, func_8025DB80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct SequenceResource {
    char pad[0x2B50];
    void *sections;
    char pad2B54[0xC];
    void *lookup;
    s32 count;
} SequenceResource;
typedef struct SequenceState {
    SequenceResource *resource;
    char pad4[0x20];
    s32 command;
    char pad28[8];
    f32 duration;
} SequenceState;
typedef struct SequenceCommand { s32 command; u16 duration; } SequenceCommand;
extern s32 func_00266D24(void *, s32, s32);
extern void *func_002017D4(void *, s32);
extern f32 func_0026D210(s32);
extern f32 D_800A81EC;
void *func_00275270(SequenceState *state, s32 index) {
    s32 selection;
    s32 section;
    void *result;
    SequenceCommand *command;
    f32 duration;
    selection = func_00266D24(state->resource->lookup, state->resource->count, index);
    if (selection != -1) {
        section = selection * 2;
        result = func_002017D4(state->resource->sections, section | 1);
        command = func_002017D4(state->resource->sections, section);
        state->command = command->command;
        duration = func_0026D210(command->duration);
        state->duration = duration;
        if (duration <= 0.0f) state->duration = D_800A81EC;
        return result;
    }
    return ((void *)0);
}
