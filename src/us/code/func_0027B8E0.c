/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AADCC_de_5.c, func_802AB1B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
extern void func_0027B8E0(u16 *grid, f32 x0, f32 y, f32 sx, f32 sy);
extern void func_0027A4DC(u32 *data, f32 x, f32 y, u32 w, u32 h, f32 sx, f32 sy);
void func_0027B8E0(u16 *grid, f32 x0, f32 y, f32 sx, f32 sy) {
    u32 *tile;
    f32 rows;
    f32 cols;
    f32 x;
    u32 w;
    u32 h;
    tile = (u32 *)(grid + 4);
    h = 0;
    rows = grid[1];
    while (rows-- != 0.0f) {
        x = x0;
        cols = grid[0];
        while (cols-- != 0.0f) {
            w = tile[0];
            h = tile[1];
            func_0027A4DC(tile + 2, x, y, w, h, sx, sy);
            x += w * sx;
            tile = (u32 *)((char *)tile + ((w * h) >> 1));
            tile += 2;
        }
        y += h * sy;
    }
}
