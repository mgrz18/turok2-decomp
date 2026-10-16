/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de_3.c, func_80261180_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Track_func_80261180_de;
struct Track_func_80261180_de {
    s32 keys;
    char pad4[0x2C - 4];
    s32 stride;
};
extern void func_00218620(char **stream, f32 *out, s32 frames);
struct Track_func_80261180_de;
typedef struct Track_func_80261180_de Track_func_80261180_de;
extern void func_00217450(Track_func_80261180_de *track, char **cursor);
extern f32 func_0021852C(Track_func_80261180_de *track, f32 time, s32 arg2, s32 arg3);
void func_00218620(char **stream, f32 *out, s32 frames)
{
    Track_func_80261180_de track;
    char *cursor;
    s32 i;
    cursor = *stream;
    func_00217450(&track, &cursor);
    for (i = 0; i < frames; i++) {
        *out = func_0021852C(&track, i, 0, 0);
        out += 4;
    }
    cursor += track.stride * (track.keys * 4 + 6);
    *stream = cursor;
}
