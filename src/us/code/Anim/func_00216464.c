/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_6.c, func_8025EF34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/object.h"

extern void Anim_DecodeQuantizedChannel(u32 *stream, Output8025EF54 *out, s32 count);
extern float Bits_Normalize(float value, int bits);
extern u32 Bits_Read(u32 *, u32);
extern f32 Math_Remap(f32 value, f32 lower, f32 upper);
/* Bits_ReadFloat with its Func802608ECResult range as the three words it travels in (a1-a3);
 * the port's Bits_ReadFloat_3 (port/game/t2abi.c) builds the struct (docs/port/ABI.md) */
#ifdef TARGET_PORT
extern f32 Bits_ReadFloat_3(u32 *stream, s32 bits, f32 base, f32 range);
#else
extern f32 Bits_ReadFloat_3(u32 *stream, s32 bits, f32 base, f32 range) __asm__("Bits_ReadFloat");
#endif
void Anim_DecodeQuantizedChannel(u32 *stream, Output8025EF54 *out, s32 count) {
    Decode8025EF54 decode;
    f32 first;
    f32 second;
    s32 i;
    f64 converted;
    decode.stream = *stream;
    if (sAnimRawRange != 0) {
        decode.first_bits = Bits_Read(&decode.stream, 0x20);
        decode.second_bits = Bits_Read(&decode.stream, 0x20);
        first = *(f32 *)&decode.first_bits;
        second = *(f32 *)&decode.second_bits;
    } else {
        decode.first_bits = Bits_Read(&decode.stream, 6);
        decode.second_bits = Bits_Read(&decode.stream, 6);
        converted = (f64)decode.first_bits;
        if (decode.first_bits < 0) {
            converted += 4294967296.0;
        }
        first = Bits_Normalize((f32)converted, 6);
        converted = (f64)decode.second_bits;
        if (decode.second_bits < 0) {
            converted += 4294967296.0;
        }
        second = Bits_Normalize((f32)converted, 6);
        first = Math_Remap(first, sAnimRemapMin, sAnimRemapMax);
        second = Math_Remap(second, sAnimRemapMin, sAnimRemapMax);
    }
    decode.base = first;
    decode.range = second - first;
    decode.bits = Bits_Read(&decode.stream, 6);
    i = 0;
    if (count > 0) {
        do {
            out->value = Bits_ReadFloat_3(&decode.stream, decode.bits,
                                       decode.base, decode.range);
            i++;
            out++;
        } while (i < count);
    }
    *stream = decode.stream;
}
