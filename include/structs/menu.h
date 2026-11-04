#ifndef STRUCTS_MENU_H
#define STRUCTS_MENU_H

/* The menu system: menu windows built from a description (MenuDesc, a table
 * of MenuItemDesc), their items, the pool of open windows and the
 * controller a window reads. Layouts from the menu VM (func_00413230.c,
 * copied as func_0044C230.c) and the item callbacks; names after Rage Wars
 * (CMenuItem_t and WidgetDescriptor in references/rw_abu/include/MENU/OBJECT1C8.H,
 * ControllerProfile in rw/abu_types.h) where it has them.
 * An item callback is called as callback(MenuItem *item, Menu *menu, ...).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

struct Menu;
struct MenuItem;
struct MenuItemLayout;
struct MenuPool;
struct CCamera;

/* One entry of a MenuDesc's item table (Rage Wars' WidgetDescriptor):
 * func_00414EA8 copies it into the MenuItem it makes. */
typedef struct MenuItemDesc {
    /* 0x00 */ s16 type;            /* 3: a model item (gets a 0x330-byte extra) */
    /* 0x02 */ u8 _pad2[2];
    /* 0x04 */ u32 flags;
    /* 0x08 */ u16 x;
    /* 0x0A */ u16 y;
    /* 0x0C */ u8 r;
    /* 0x0D */ u8 g;
    /* 0x0E */ u8 b;
    /* 0x0F */ u8 a;
    /* 0x10 */ s32 data;            /* the item's starting data (its text...) */
    /* 0x14 */ void (*draw)(struct MenuItem *item, struct MenuItemLayout *layout, s32 arg, struct Menu *menu);
    /* 0x18 */ void (*update)(struct MenuItem *item, struct Menu *menu, void *arg);
    /* 0x1C */ s32 (*select)(struct MenuItem *item, struct Menu *menu, void *arg);
    /* 0x20 */ s16 param;           /* free for the item's callbacks (the inventory menus: an item type) */
    /* 0x22 */ s16 index;           /* (the inventory menus: which one of that type) */
} MenuItemDesc; /* size 0x24 */

/* A window's item (Rage Wars' CMenuItem_t), made from a MenuItemDesc; a
 * window's items follow it in memory. */
typedef struct MenuItem {
    /* 0x00 */ s32 index;           /* position in the window */
    /* 0x04 */ s16 type;            /* the desc's type */
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u16 x;
    /* 0x0E */ u16 y;
    /* 0x10 */ u8 r;
    /* 0x11 */ u8 g;
    /* 0x12 */ u8 b;
    /* 0x13 */ u8 a;
    union {
    /* 0x14 */ char *text;          /* a text item's line (often a sprintf buffer) */
    /* 0x14 */ s32 data;
    /* 0x14 */ s32 image;           /* an image item's image */
    };
    /* 0x18 */ MenuItemDesc *def;
    /* 0x1C */ s32 value;           /* -1 when made; a slider's position, 0-0xFF */
    /* 0x20 */ void *extra;         /* the 0x330-byte model of a type-3 item */
    /* 0x24 */ struct Menu *menu;   /* the window it belongs to */
} MenuItem; /* size 0x28 */

/* Where an item is drawn: func_0041363C lays an item out into one, the
 * draw callbacks get it (Rage Wars' MenuOptionBox). */
typedef struct MenuItemLayout {
    /* 0x00 */ s32 mode;
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

/* The controller a window reads: a view of Rage Wars' ControllerProfile
 * (rw/abu_types.h). */
typedef struct MenuController {
    /* 0x00 */ u8 _pad0[4];
    /* 0x04 */ s8 port;
    /* 0x05 */ u8 _pad5[0xB0 - 0x5];
    /* 0xB0 */ u32 buttons;
    /* 0xB4 */ s32 connection;
    /* 0xB8 */ u8 _padB8[4];
    /* 0xBC */ s32 history;
} MenuController;

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
    /* 0x018 */ s32 param;
    /* 0x01C */ void *player;       /* the CPlayer (structs/player.h) it is for */
    /* 0x020 */ MenuController *controller;
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

#endif
