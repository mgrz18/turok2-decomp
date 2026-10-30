/* WRONG CUT: real function 0x00416B30, size 0x7C (through 0x00416BAC): splat cut the loop
 * off as func_00416B44. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ u8 padB8[4];
    /* 0xBC */ s32 unkBC;
} MenuController;

typedef struct Menu Menu;
typedef struct MenuPool MenuPool;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ void (*close)(Menu *menu, MenuPool *pool);
} MenuTmpl;

struct Menu {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void *mem;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ MenuTmpl *tmpl;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ MenuController *ctl;
};

struct MenuPool {
    /* 0x00 */ Menu *head;
};

extern void CList__Remove(MenuPool *list, Menu *item);
extern void func_002052D8(s32, void *mem);

/* Closes and frees every open menu. */
void func_00416B30(MenuPool *pool) {
    Menu *menu;
    MenuController *ctl;

next:
    menu = pool->head;
    if (menu != NULL) {
        if (menu->tmpl->close != NULL) {
            menu->tmpl->close(menu, pool);
        }
        ctl = menu->ctl;
        ctl->unkB0 = 0;
        ctl->unkB4 = 0;
        ctl->unkBC = 0;
        CList__Remove(pool, menu);
        func_002052D8(0, menu->mem);
        goto next;
    }
}
