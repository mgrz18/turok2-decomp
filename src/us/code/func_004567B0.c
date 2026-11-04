/* 0x004567B0-0x00456948 (linked 0x0041D7B0, where its callers jal); the word at
 * 0x004567AC is junk between objects (src/us/asm/stray_004567AC.s).
 * Second-module twin of func_0041D890. */
#include "common.h"
#include "structs/game.h"
#include "structs/save.h"

/* Build the save block: the player's profile, the multiplayer settings and the
 * player's options are copied into a freshly allocated buffer. */

/* The end of the function reads the size through a struct view (address in a
 * register, reloaded after the extraSize store); the top reads it as a scalar. */
typedef struct {
    s32 size;
} ExtraInfo;

extern s32 D_800F70A8; /* size of the extra data */

/* Struct pointee: the load of ->data is scheduled before the D_800C2010 store. */
typedef struct {
    void *data;
} MemHandle;

extern MemHandle *D_800C2010; /* handle of the save block */
extern u8 D_800EBAC0[];
extern char D_800C13F0[];

void func_002053A8(s32 heap);
void func_002052D8(s32 heap, MemHandle *handle);
MemHandle *func_00204EDC(s32 heap, s32 size, s32 tag, char *name);
void func_00266C5C(void *dst, s32 value, s32 size);
void func_0029E460(void *dst, void *src, s32 size);

void func_004567B0(CPlayer *player) {
    s32 size = D_800F70A8 + 0x688;
    s32 allocSize;
    SaveGame *data;

    if (D_800C2010 != NULL) {
        func_002053A8(0);
        func_002052D8(0, D_800C2010);
    }
    allocSize = D_800F70A8 + 0x688;
    D_800C2010 = func_00204EDC(0, allocSize, 0x33, D_800C13F0);
    D_800C2018 = D_800C2010->data;
    func_00266C5C(D_800C2018, 0, allocSize);
    D_800C2018->player = player->m_Data;
    D_800C2018->settings = *(SaveSettings *)&D_8011AAD8;
    data = D_800C2018;
    data->frames = D_8011ACB0.frames;
    data->seconds = D_8011ACB0.seconds;
    data->minutes = D_8011ACB0.minutes;
    data->hours = D_8011ACB0.hours;
    data->size = size;
    data->levelFlagsSize = ((ExtraInfo *) &D_800F70A8)->size;
    func_0029E460(data->levelFlags, D_800EBAC0, ((ExtraInfo *) &D_800F70A8)->size);
}
