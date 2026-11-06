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
extern unsigned short sAdecBitsLeft;
extern unsigned short sAdecHuffBits;
extern unsigned short sAdecHuffBitsAvail;
extern s16 sAdecHeaderDone;
extern unsigned short sAdecImdctReady;
extern char sAdecState;
u32 adecStreamOpen(s32 *source, u32 divisor, s32 *first)
{
    DecodeState *state = &sAdecState;
    state->source = source;
    state->cursor = source;
    state->first = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    state->count = *(s32 *)state->cursor;
    state->second = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    state->third = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    sAdecHeaderDone = 0;
    sAdecImdctReady = 0;
    sAdecBitsLeft = 0;
    sAdecHuffBits = 0;
    sAdecHuffBitsAvail = 0;
    *first = state->first;
    return (u64)((s64)state->count * 0xF424000) / divisor;
}
