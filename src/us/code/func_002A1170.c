/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/AUDIO/802C3EA4_3.CPP, func_802C00B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef struct DecodeState {
    s32 first;
    s32 third;
    void *source;
    s32 second;
    s32 count;
    void *cursor;
    u16 frame[336];
} DecodeState;
extern unsigned short D_800B8BA8;
extern unsigned short D_800B8BAA;
extern unsigned short D_800B8BAC;
extern s16 D_800B8BAE;
extern unsigned short D_800B8BB0;
extern char D_80125C38;
u32 func_002A1170(s32 *source, u32 divisor, s32 *first)
{
    DecodeState *state = &D_80125C38;
    state->source = source;
    state->cursor = source;
    state->first = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    state->count = *(s32 *)state->cursor;
    state->second = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    state->third = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    D_800B8BAE = 0;
    D_800B8BB0 = 0;
    D_800B8BA8 = 0;
    D_800B8BAA = 0;
    D_800B8BAC = 0;
    *first = state->first;
    return (u64)((s64)state->count * 0xF424000) / divisor;
}
