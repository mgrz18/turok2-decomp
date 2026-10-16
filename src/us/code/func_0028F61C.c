/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE6BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct DecodeResult_func_802AE6BC_de;
struct DecodeResult_func_802AE6BC_de {
    s16 type;
    u8 pad2[2];
    u32 value;
    u8 pad8[8];
};
struct RuntimeState;
typedef struct RuntimeState RuntimeState;
struct RuntimeState;
struct RuntimeState {
    void *resources;
    u32 active;
    u32 scale;
    u32 total;
    u32 previous;
    u32 first;
    u8 *objects[16];
    u32 values[16];
    u8 flags98[16];
    u8 flagsA8[16];
    u32 results[16];
};
struct DecodeResult;
struct DecodeResult {
    s16 type;
    u8 pad2[6];
    u8 status;
    u8 first_data;
    u8 second_data;
    u8 extra1;
    u32 extra2;
};
struct DecodeResult_func_802AE6BC_de;
typedef struct DecodeResult_func_802AE6BC_de DecodeResult_func_802AE6BC_de;
struct DecodeResult;
typedef struct DecodeResult DecodeResult;
void func_0028F61C(RuntimeState *state, DecodeResult_func_802AE6BC_de *result) {
    extern u32 func_0028F98C(RuntimeState *state, u32 index);
    extern s32 func_0028EF90(RuntimeState *state, u32 index, DecodeResult *result);
    u32 minimum;
    u32 index;
    u32 selected;
    u32 decrement;
    minimum = -1;
    index = 0;
    decrement = state->previous;
    do {
        if ((state->active >> index) & 1) {
            if (state->first != 0) {
                state->results[index] -= decrement;
            }
            if (state->results[index] < minimum) {
                minimum = state->results[index];
                selected = index;
            }
        }
        index++;
    } while (index < 16);
    func_0028EF90(state, selected, result);
    result->value = minimum;
    state->previous = minimum;
    state->total += minimum;
    if (result->type != 0x12) {
        u32 delta;
        delta = func_0028F98C(state, selected);
        state->results[selected] += delta;
    }
    state->first = 1;
}
