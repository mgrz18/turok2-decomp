#ifndef STRUCTS_MENU_H
#define STRUCTS_MENU_H

/* The menu system: menu windows built from a description (MenuDesc, a table
 * of MenuItemDesc), their items, the pool of open windows and the
 * controller a window reads. Layouts from the menu VM (func_00413230.c,
 * copied as func_0044C230.c) and the item callbacks; names after Rage Wars
 * (CMenuItem_t and WidgetDescriptor in references/rw_abu/include/MENU/OBJECT1C8.H,
 * ControllerProfile in structs/controller.h) where it has them.
 * An item callback is called as callback(MenuItem *item, Menu *menu, ...).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "structs/controller.h"

struct Menu;
struct MenuDesc;
struct MenuItem;
struct MenuItemLayout;
struct MenuPool;
struct MenuModel;
struct MenuModelPart;
struct CCamera;

/* An item's type (MenuItemDesc / MenuItem type), as func_0041363C lays them
 * out: 0 text, 1 image, 2 slider (label, bar and knob: value 0-0xFF), 3 a
 * 3D model (MenuModelPart data, a MenuModel extra), 4 one character, 5 a
 * bound value (MenuBinding data, shown as "name value"). */

/* The links of an item (MenuItemDesc / MenuItem link[]): how many items
 * the cursor moves (a signed byte) on left, right, up and down
 * (func_004151B4, called with 0x10 + the direction: the byte at that offset). */
#define MENU_LINK_LEFT 0
#define MENU_LINK_RIGHT 1
#define MENU_LINK_UP 2
#define MENU_LINK_DOWN 3

/* One entry of a MenuDesc's item table (Rage Wars' WidgetDescriptor):
 * func_00414EA8 copies it into the MenuItem it makes. */
typedef struct MenuItemDesc {
    /* 0x00 */ s16 type;            /* 3: a model item (gets a 0x330-byte extra) */
    /* 0x02 */ u8 _pad2[2];
    /* 0x04 */ u32 flags;
    /* 0x08 */ u16 x;
    /* 0x0A */ u16 y;
    /* 0x0C */ u8 link[4];          /* MENU_LINK_*: the cursor's moves, signed */
    /* 0x10 */ s32 data;            /* the item's starting data (its text...) */
    /* 0x14 */ void (*draw)(struct MenuItem *item, struct MenuItemLayout *layout, struct MenuItemLayout *area, struct Menu *menu);
    /* 0x18 */ void (*update)(struct MenuItem *item, struct Menu *menu, void *arg);
    /* 0x1C */ s32 (*select)(struct MenuItem *item, struct Menu *menu, void *arg);
    /* 0x20 */ s16 param;           /* free for the item's callbacks (the inventory menus: an item type) */
    /* 0x22 */ s16 index;           /* (the inventory menus: which one of that type) */
} MenuItemDesc; /* size 0x24 */

/* A type-5 item's data (Rage Wars' slider binding): a variable of any
 * numeric type, found by storage(item), shown as "name value / scale"
 * (func_00413230) and stepped by left/right within min..max (func_004133A4). */
typedef struct MenuBinding {
    /* 0x00 */ char *name;
    /* 0x04 */ s32 type;            /* 0 f32, 1/2/5 s32, 3 s8, 4 s16, 6 u8, 7 u16, 8 u32 */
    /* 0x08 */ f32 min;
    /* 0x0C */ f32 max;
    /* 0x10 */ f32 step;
    /* 0x14 */ f32 scale;
    /* 0x18 */ void *(*storage)();  /* storage(item) */
} MenuBinding; /* size 0x1C */

/* A window's item (Rage Wars' CMenuItem_t), made from a MenuItemDesc; a
 * window's items follow it in memory. */
typedef struct MenuItem {
    /* 0x00 */ s32 index;           /* position in the window */
    /* 0x04 */ s16 type;            /* the desc's type */
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ s16 x;               /* the desc's (read signed: func_0041363C) */
    /* 0x0E */ s16 y;
    /* 0x10 */ u8 link[4];          /* MENU_LINK_*: the cursor's moves, signed */
    union {
    /* 0x14 */ char *text;          /* a text item's line (often a sprintf buffer) */
    /* 0x14 */ s32 data;
    /* 0x14 */ s32 image;           /* an image item's image */
    /* 0x14 */ s32 *table;          /* a list item's table (set by its update callback) */
    /* 0x14 */ MenuBinding *binding; /* a type-5 item's */
    /* 0x14 */ struct MenuModelPart *part; /* a type-3 item's (structs/menu_model.h) */
    };
    /* 0x18 */ MenuItemDesc *def;
    /* 0x1C */ s32 value;           /* -1 when made; a slider's position, 0-0xFF */
    union {
    /* 0x20 */ void *extra;
    /* 0x20 */ struct MenuModel *model; /* the 0x330-byte model of a type-3 item (structs/menu_model.h) */
    };
    /* 0x24 */ struct Menu *menu;   /* the window it belongs to */
} MenuItem; /* size 0x28 */

/* Where an item is drawn: func_0041363C lays an item out into one, the
 * draw callbacks get it (Rage Wars' MenuOptionBox). func_004156FC lays out
 * every item and hands each draw callback its layout and the window's area
 * (the box around all the items). */
typedef struct MenuItemLayout {
    /* 0x00 */ s32 index;           /* the item's index (the selected one when it is the window's cursor) */
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ s32 left;
    /* 0x18 */ s32 right;
    /* 0x1C */ s32 top;
    /* 0x20 */ s32 bottom;
} MenuItemLayout; /* size 0x24 */

/* What describes a menu window: its item table and callbacks. */
typedef struct MenuDesc {
    /* 0x00 */ MenuItemDesc *items;
    /* 0x04 */ s16 count;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ void (*init)(struct Menu *menu, struct MenuPool *pool);
    /* 0x0C */ s32 (*close)();
    /* 0x10 */ s32 (*update)();
    /* 0x14 */ s32 (*poll)(void);
    /* 0x18 */ u16 cursor;          /* the window's first cursor */
    /* 0x1A */ u16 offsetX;
    union {
    /* 0x1C */ u16 offsetY;
    /* 0x1C */ s32 value;           /* what func_0041647C returns */
    };
    /* 0x20 */ s32 param;           /* handed to the window as its param */
} MenuDesc; /* size 0x24 */

/* How a text item's class is drawn: the item's flags & 0x3FE0 (0x20 to
 * 0x2000, one bit each) pick one of the nine D_800C1BD4 (func_00416EA8). */
typedef struct MenuTextStyle {
    /* 0x00 */ s32 font;            /* 0, 1, 2: the draw mode (D_800C1BD0) set for it */
    /* 0x04 */ s32 charWidth;
    /* 0x08 */ s32 lineHeight;      /* in 222ths of the screen */
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ u8 shadow;
    /* 0x15 */ u8 r0;               /* the font colours (COnScreen__SetFontColor), dimmed unless selected */
    /* 0x16 */ u8 g0;
    /* 0x17 */ u8 b0;
    /* 0x18 */ u8 r1;
    /* 0x19 */ u8 g1;
    /* 0x1A */ u8 b1;
    /* 0x1B */ u8 _pad1B;
} MenuTextStyle; /* size 0x1C */

extern MenuTextStyle D_800C1BD4[9];

/* One of a window's 2x8 colour records, from the table D_800C1CD2. */
typedef struct MenuColor {
    /* 0x00 */ u16 w0;
    /* 0x02 */ u16 w2;
    /* 0x04 */ u16 w4;
    /* 0x06 */ u16 w6;
    /* 0x08 */ u16 w8;
    /* 0x0A */ u16 wA;
    /* 0x0C */ u8 bC;
    /* 0x0D */ u8 bD;
    /* 0x0E */ u8 bE;
    /* 0x0F */ u8 bF;
} MenuColor; /* size 0x10 */

/* An open menu window, 0x1D8 bytes followed by its items (and their
 * extras); func_00414EA8 builds it. Linked into a MenuPool by prev/next. */
typedef struct Menu {
    /* 0x000 */ s16 cursor;         /* the selected item */
    /* 0x002 */ s16 offsetX;
    /* 0x004 */ s16 offsetY;
    /* 0x006 */ u8 _pad6[2];
    /* 0x008 */ void *block;        /* the memory block it lives in */
    /* 0x00C */ MenuItem *items;
    /* 0x010 */ s32 count;
    /* 0x014 */ MenuDesc *desc;
    union {
    /* 0x018 */ s32 param;
    /* 0x018 */ struct MenuDesc *submenu; /* what func_00415FB8 opens from it */
    };
    /* 0x01C */ struct CPlayer *player; /* structs/player.h */
    /* 0x020 */ ControllerProfile *controller; /* structs/controller.h */
    union {
    /* 0x024 */ void *context;      /* the opener's argument */
    /* 0x024 */ char *text;
    };
    /* 0x028 */ s16 state;
    /* 0x02A */ s16 slot;           /* the pool's opened count when made */
    /* 0x02C */ f32 time;
    /* 0x030 */ f32 fade;
    /* 0x034 */ f32 alpha;
    /* 0x038 */ s32 visible;
    /* 0x03C */ s32 highlighted;
    /* 0x040 */ struct CCamera *camera;
    /* 0x044 */ struct MenuPool *pool;
    /* 0x048 */ MenuColor colors[2][8];
    /* 0x148 */ u8 _pad148[0x1C8 - 0x148];
    /* 0x1C8 */ s32 flash;
    /* 0x1CC */ s32 flashStep;
    /* 0x1D0 */ struct Menu *prev;
    /* 0x1D4 */ struct Menu *next;
} Menu; /* size 0x1D8 */

/* The open windows: a CList (links at 0x1D0/0x1D4) and a counter. */
typedef struct MenuPool {
    /* 0x00 */ Menu *head;
    /* 0x04 */ Menu *tail;
    /* 0x08 */ s32 prevOffset;
    /* 0x0C */ s32 nextOffset;
    /* 0x10 */ s32 count;
    /* 0x14 */ s16 opened;
    /* 0x16 */ u8 _pad16[2];
} MenuPool; /* size 0x18 */

/* A line of text being typed in a menu: the letters, where the next one
 * goes and the blinking cursor (drawn while timer <= 6, func_0041A904).
 * D_800C1F80 is the cheat code entry (22 letters at most; its text also
 * has a symbol of its own, D_800C1F8C); D_800C1FB0 holds the four players'
 * names (8 letters at most), indexed by the player's slot (func_0025340C). */
typedef struct TextEntry {
    /* 0x00 */ s32 cursor;          /* the column the next letter goes to */
    /* 0x04 */ f32 timer;           /* the cursor's blink, reset by each key */
    /* 0x08 */ s32 length;
    /* 0x0C */ char text[24];
} TextEntry; /* size 0x24 */

typedef struct NameEntry {
    /* 0x00 */ s32 cursor;
    /* 0x04 */ f32 timer;
    /* 0x08 */ s32 length;
    /* 0x0C */ char name[12];
} NameEntry; /* size 0x18 */

extern TextEntry D_800C1F80;
extern NameEntry D_800C1FB0[4];

#endif
