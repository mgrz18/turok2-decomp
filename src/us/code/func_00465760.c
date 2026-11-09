/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/8044E2B8.CPP, func_8044D668_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/scene.h"
struct Item_func_8044D668_de;
typedef struct Item_func_8044D668_de Item_func_8044D668_de;
struct Item_func_8044D668_de;
struct Item_func_8044D668_de {
    char pad0[0x11];
    u8 kind;
    char pad12[2];
};
struct Level;
struct Level {
    char pad0[0x1168]; /* 0x11C0 in Rage Wars */
    s32 counts[2];
    char pad11C8[8];
    Item_func_8044D668_de *items[2];
};
struct Level;
typedef struct Level Level;
extern void Cinema_Disable();
extern void Cinema_Enable();
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
void func_00465760(Level *level) {
    s32 i;
    s32 n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    n = level->counts[0];
    for (i = 0; i < n; i++) {
        item = &level->items[0][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    n = level->counts[1];
    for (i = 0; i < n; i++) {
        item = &level->items[1][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    Cinema_Enable();
}
