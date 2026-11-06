#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/save.h"

typedef struct {
    /* 0x0 */ s32 size;
} SaveSize;

extern SaveSize D_800F70A8;
extern s32 *D_800C2010;        /* buffer handle */
extern char D_800C13F0[];
extern char D_800EBAC0[];
extern void CCache__Flush(s32);
extern void CCache__Deallocate(s32, void *);
extern s32 *CCache__Allocate(s32, s32, s32, void *);
extern void memset(void *dst, s32 value, s32 size);
extern void func_0029E460(char *dst, char *src, s32 n);

static __inline__ s32 SaveFileSize(void) {
    return D_800F70A8.size + 0x688;
}

/* Snapshots the player and the multiplayer setup into a freshly allocated buffer. */
void SaveGame_Build(CPlayer *p) {
    s32 size = SaveFileSize();
    SaveGame *save;
    s32 allocSize;
    s32 *handle;
    SaveGame *buf;

    if (D_800C2010 != NULL) {
        CCache__Flush(0);
        CCache__Deallocate(0, D_800C2010);
    }
    allocSize = SaveFileSize();
    handle = CCache__Allocate(0, allocSize, 0x33, D_800C13F0);
    buf = (SaveGame *) *handle;
    D_800C2010 = handle;
    D_800C2018 = buf;
    memset(D_800C2018, 0, allocSize);
    D_800C2018->player = p->m_Data;
    D_800C2018->settings = *(GameSettingsBlock *)&D_8011AAD8;
    save = D_800C2018;
    save->frames = D_8011ACB0.frames;
    save->seconds = D_8011ACB0.seconds;
    save->minutes = D_8011ACB0.minutes;
    save->hours = D_8011ACB0.hours;
    /* D_800F70A8 is a struct field here, so it is reloaded after the store into the header. */
    save->size = size;
    save->levelFlagsSize = D_800F70A8.size;
    func_0029E460((char *)save->levelFlags, D_800EBAC0, D_800F70A8.size);
}
