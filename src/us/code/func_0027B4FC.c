/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AADCC_de_4.c, func_802AADCC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern void func_0027B4FC(u16 *grid, s16 x0, s32 y);
extern void func_00279158(s32 *pixels, char *mask, s32 x, s16 y, s32 w, s32 h);
void func_0027B4FC(u16 *grid, s16 x0, s32 y) {
    s32 *tile;
    s32 rows;
    s32 cols;
    s32 x;
    s32 w;
    s32 h;
    u32 area;
    tile = (s32 *)(grid + 4);
    h = 0;
    rows = grid[1];
    while (--rows != -1) {
        x = x0;
        cols = grid[0];
        while (--cols != -1) {
            w = tile[0];
            h = tile[1];
            area = w * h;
            func_00279158(tile + 2, (char *)tile + (area * 2 + 8), x, y, w, h);
            x += w;
            tile = (s32 *)((char *)tile + (area * 2 + (area >> 1)));
            tile += 2;
        }
        y += h;
    }
}
