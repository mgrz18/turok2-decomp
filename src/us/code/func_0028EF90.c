/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802AD870_7.CPP, func_802AE030_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
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
struct DecodeResult;
typedef struct DecodeResult DecodeResult;
struct MeasuredRecAt18 { unsigned char padding[24]; void * value; };
struct MeasuredRecAt0V3 { u8 value; };
extern s32 func_0028F460(s32, s32);
extern u32 func_0028F98C(RuntimeState *, u32);
s32 __alCSeqGetTrackEvent(RuntimeState *state, u32 index, DecodeResult *result) {
    u8 first;
    u8 command;
    u32 first_value;
    u32 command_value;
    first = func_0028F460(state, index);
    first_value = first & 0xFF;
    if (first_value == 0xFF) {
        command = func_0028F460(state, index);
        command_value = command & 0xFF;
        if (command_value == 0x51) {
            result->type = 3;
            result->status = first;
            result->first_data = command;
            result->extra1 = func_0028F460(state, index);
            ((u8 *)&result->extra2)[0] = func_0028F460(state, index);
            ((u8 *)&result->extra2)[1] = func_0028F460(state, index);
            state->flagsA8[index] = 0;
        } else if (command_value == 0x2F) {
            u32 active = state->active ^ (1U << index);
            state->active = active;
            if (active != 0) {
                result->type = 0x12;
            } else {
                result->type = 4;
            }
        } else if (command_value == 0x2E) {
            func_0028F460(state, index);
            func_0028F460(state, index);
            state->flagsA8[index] = 0;
            result->type = 0x13;
        } else if (command_value == 0x2D) {
            void *base;
            void *p;
            u8 lead;
            u8 count_byte;
            u32 count;
            u32 byte0;
            u32 byte1;
            u32 byte2;
            u32 byte3;
            void *next;
            base = (u8 *)state + (index * 4);
            p = ((struct MeasuredRecAt18 *)(base))->value;
            lead = ((struct MeasuredRecAt0V3 *)(p))->value;
            p = (u8 *)p + 1;
            count_byte = ((struct MeasuredRecAt0V3 *)(p))->value;
            count = count_byte & 0xFF;
            if (count == 0) {
                next = (u8 *)p + 5;
                ((struct MeasuredRecAt0V3 *)(p))->value = lead;
            } else {
                if (count != first_value) {
                    ((struct MeasuredRecAt0V3 *)(p))->value = count_byte - 1;
                }
                p = (u8 *)p + 1;
                byte0 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte1 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte2 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte3 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                next = (u8 *)p - ((byte0 << 24) + (byte1 << 16) +
                                   (byte2 << 8) + byte3);
            }
            ((struct MeasuredRecAt18 *)(base))->value = next;
            state->flagsA8[index] = 0;
            result->type = 0x14;
        }
    } else {
        result->type = 1;
        if (first & 0x80) {
            result->status = first;
            result->first_data = func_0028F460(state, index);
            state->flagsA8[index] = first;
        } else {
            u8 running_status = state->flagsA8[index];
            result->first_data = first;
            result->status = running_status;
        }
        command_value = result->status & 0xF0;
        if ((command_value != 0xC0) && (command_value != 0xD0)) {
            result->second_data = func_0028F460(state, index);
            if ((result->status & 0xF0) == 0x90) {
                result->extra2 = func_0028F98C(state, index);
            }
        } else {
            result->second_data = 0;
        }
    }
    return 1;
}
