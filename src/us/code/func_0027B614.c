/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002ABED4.c, func_002ABED4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00279528(u8 *, u8 *, f32, f32, u32, u32, f32, f32);
void func_0027B614(u16 *hdr, f32 x0, f32 y, f32 sx, f32 sy) {
    f32 rows, cols, x;
    u32 w, h;
    u8 *p;
    p = (u8 *)hdr + 8;
    h = 0;
    rows = hdr[1];
    while (rows-- != 0.0f) {
        cols = hdr[0];
        x = x0;
        while (cols-- != 0.0f) {
            h = ((AtlasCell *)p)->h;
            w = ((AtlasCell *)p)->w;
            func_00279528(p + 8, p + (w * h * 2 + 8), x, y, w, h, sx, sy);
            x += (f32)w * sx;
            p += (w * h * 2) + ((w * h) >> 1);
            p += 8;
        }
        y += (f32)h * sy;
    }
}
