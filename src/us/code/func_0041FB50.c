#include "common.h"
#include "structs/menu.h"

/* Save/load menu: fills a pak file line ("NAME.EXT" and its size) for the
 * note of the chosen pak; an empty or unreadable note shows blanks. */

extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2028;
extern char D_800B48D4[];
extern char D_800B48E0[];
extern char D_800C1404[];

extern s32 func_00425E74(s32 slot, s32 note, s32 *used, char *name, char *ext, s32 *size, void *a6, void *a7);
extern s32 func_00426E3C(char *s, s32 n);
extern void func_00426E04(char *dst, char *src);
extern void func_0029E3E0(char *dst, char *fmt, s32 value);

s32 func_0041FB50(MenuItem *item, Menu *c) {
    char name[16];
    char ext[8];
    u8 unk38[8];
    u8 unk40[8];
    s32 used;
    s32 size;
    char ch = 0x20;
    s32 note = item->index - 3;
    s32 slot;
    s32 more;
    s32 r;
    s32 i;
    char *dst;

    if (D_801309BC != 0) {
        return 0;
    }
    if (D_80130990 != 0) {
        slot = D_800C2028;
    } else {
        slot = c->controller->port;
    }
    r = func_00425E74(slot, note, &used, name, ext, &size, unk38, unk40);
    if (used != 0 && r == 0) {
        if (func_00426E3C(name, 16) != 0) {
            func_00426E04(name, D_800B48D4);
        }
    } else {
        func_00426E04(name, D_800B48E0);
        func_00426E04(ext, D_800B48E0);
        size = 0;
    }
    /* copy the name and the extension, padding with spaces after the terminator */
    dst = item->text;
    dst += 3;
    more = 1;
    for (i = 0; i < 16; i++) {
        if (more) {
            ch = name[i];
        }
        if (ch == 0) {
            more = 0;
            ch = 0x20;
        }
        *dst++ = ch;
    }
    more = 1;
    dst += more;
    for (i = 0; i < 4; i++) {
        if (more) {
            ch = ext[i];
        }
        if (ch == 0) {
            more = 0;
            ch = 0x20;
        }
        *dst++ = ch;
    }
    *dst += 2;
    func_0029E3E0(dst, D_800C1404, size);
    return 0;
}
