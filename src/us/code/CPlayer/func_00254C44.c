/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388_7.CPP, func_8021836C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/player.h"

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
struct Menu_254C44;
struct Menu_254C44 {
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
struct Menu_254C44;
typedef struct Menu_254C44 Menu_254C44;
struct Menu_254C44;
extern void CBarrel__Reset(Menu_254C44 *menu);
void CBarrel__Reset(Menu_254C44 *menu) {
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
        menu->slots[i].angle = -(i * 0.17453295f);
        menu->slots[i].y = 0;
        menu->slots[i].scale = gBarrelSlotScale;
    }
}
