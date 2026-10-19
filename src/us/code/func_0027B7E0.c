/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AADCC_de_5.c, func_802AB0B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0027B7E0(u16 *grid, s16 x0, s32 y);
extern void func_0027A2C4(s32 *data, s32 x, s16 y, s32 w, s32 h);
void func_0027B7E0(u16 *grid, s16 x0, s32 y) {
    s32 *tile;
    s32 rows;
    s32 cols;
    s32 x;
    s32 w;
    s32 h;
    tile = (s32 *)(grid + 4);
    h = 0;
    rows = grid[1];
    while (--rows != -1) {
        x = x0;
        cols = grid[0];
        while (--cols != -1) {
            w = tile[0];
            h = tile[1];
            func_0027A2C4(tile + 2, x, y, w, h);
            x += w;
            tile = (s32 *)((char *)tile + ((u32)(w * h) >> 1));
            tile += 2;
        }
        y += h;
    }
}
