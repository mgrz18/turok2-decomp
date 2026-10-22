/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388_7.CPP, func_8021836C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
struct SlotRecAt14;
typedef struct SlotRecAt14 SlotRecAt14;
struct SlotRecAt14;
struct SlotRecAt14 {
    s32 weapon;
    void *info;
    s32 owned;
    f32 angle;
    s32 y;
    f32 scale;
};
struct Menu;
struct Menu {
    s32 active;
    s32 pad4;
    f32 open;
    s32 reset;
    u8 reserved10[4];
    f32 spin;
    s32 selected;
    SlotRecAt14 slots[36];
    s32 cursor;
    s32 count;
    u8 reserved384[4];
    s32 hover;
    s32 scroll;
    s32 last;
};
struct Menu;
typedef struct Menu Menu;
extern float D_800A7128;
extern float D_800AE2D8;
struct Menu;
extern void func_00254C44(Menu *menu);
void func_00254C44(Menu *menu) {
    s32 i;
    menu->active = 0;
    menu->pad4 = 0;
    menu->open = 0;
    menu->reset = 0;
    menu->spin = 0;
    menu->cursor = -1;
    menu->count = 1;
    menu->hover = 0; /* -1 in Rage Wars */
    menu->selected = 0;
    menu->scroll = 0;
    menu->last = -1;
    for (i = 0; i < 36; i++) {
        menu->slots[i].angle = -(i * D_800A7128);
        menu->slots[i].y = 0;
        menu->slots[i].scale = D_800AE2D8;
    }
}
