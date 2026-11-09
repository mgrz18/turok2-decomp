#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

/* Save/load menu: fills a pak file line ("NAME.EXT" and its size) for the
 * note of the chosen pak; an empty or unreadable note shows blanks. */

extern char D_800B48D4[];
extern char D_800B48E0[];

extern s32 Pak_FindNote(s32 slot, s32 note, s32 *used, char *name, char *ext, s32 *size, void *a6, void *a7);
extern s32 Pak_TextHasInvalidChar(char *s, s32 n);
extern void Str_Copy(char *dst, char *src);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);
#else
extern void sprintf(char *dst, char *fmt, s32 value);
#endif

s32 PakMenu_NoteText(MenuItem *item, Menu *c) {
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

    if (gPakMenuLeaving != 0) {
        return 0;
    }
    if (gPakMenuAnyPak != 0) {
        slot = gPakMenuPort;
    } else {
        slot = c->controller->port;
    }
    r = Pak_FindNote(slot, note, &used, name, ext, &size, unk38, unk40);
    if (used != 0 && r == 0) {
        if (Pak_TextHasInvalidChar(name, 16) != 0) {
            Str_Copy(name, D_800B48D4);
        }
    } else {
        Str_Copy(name, D_800B48E0);
        Str_Copy(ext, D_800B48E0);
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
    sprintf(dst, "%3d", size);
    return 0;
}
