/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A9700.c, func_002A9700.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef union {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} P4Gfx;
extern P4Gfx *D_800D8E24;
extern s32 D_800F56A8;
void *func_0027B4EC(void);
void func_0027845C(void) {
    void *tex = func_0027B4EC();
    if (tex != ((void *)0)) {
        D_800F56A8 = 4;
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xFD900000); _g->words.w1 = ((u32)tex); };
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xF5900100); _g->words.w1 = (0x0700C040); };
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xE6000000); _g->words.w1 = (0); };
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xF3000000); _g->words.w1 = (0x0701F800); };
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xE7000000); _g->words.w1 = (0); };
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xF5800300); _g->words.w1 = (0x0000C040); };
        { P4Gfx *_g = D_800D8E24++; _g->words.w0 = (0xF2000000); _g->words.w1 = (0x0003C01C); };
    }
}
