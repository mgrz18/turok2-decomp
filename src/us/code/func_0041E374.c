#include "common.h"
#include "globals/pak.h"

/* Pak slot scan step: a pak that already holds one of the game's two files
 * (names gPakCharacterNoteName, gPakGameNoteName) wins at once (*best = 123); otherwise the
 * slot is a candidate when the pak is usable and has room for a save, and
 * the slot with the most free pages is kept. Returns 1 to keep scanning. */

extern u8 *D_800F70A8;
extern char gPakCharacterNoteName[];
extern char gPakGameNoteName[];

extern void Pak_GetNoteGameName(s32 slot, s32 index, char **out);
extern void Pak_DecodeText(char *src, char *dst, s32 n);
extern s32 Str_Compare(char *a, char *b);
extern s32 Pak_GetFreePages(s32 slot, s32 *score);
extern s32 Pak_CountEmptyNotes(s32 slot, s32 *count);
extern s32 Pak_BytesToPages(u8 *size);

/* PakMenu_FindNoteByName without the index output */
static __inline__ s32 hasFile(char *file, s32 slot) {
    char name[16];
    char *entry;
    s32 i;

    for (i = 0; i < 16; i++) {
        Pak_GetNoteGameName(slot, i, &entry);
        Pak_DecodeText(entry, name, 16);
        if (Str_Compare(name, file) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 PakMenu_CheckSlotForSave(void *unused, s32 slot, s32 *bestSlot, s32 *best) {
    s32 free;
    s32 files;
    s32 found;
    s32 found2;
    s32 err;
    s32 need;

    found = hasFile(gPakCharacterNoteName, slot);
    found2 = hasFile(gPakGameNoteName, slot);
    if (found || found2) {
        *bestSlot = slot;
        *best = 123;
        return 1;
    }
    err = Pak_GetFreePages(gPakMenuPort, &free);
    if (err == 0) {
        err = Pak_CountEmptyNotes(gPakMenuPort, &files);
        if (err == 0) {
            need = Pak_BytesToPages(D_800F70A8 + 0x688);
            if (files == 0 || free < need) {
                if (*best < need) {
                    *best = need;
                    *bestSlot = slot;
                }
                return 0;
            }
        }
    }
    return err == 0;
}
