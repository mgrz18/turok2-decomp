/* 0x004567B0-0x00456948 (linked 0x0041D7B0, where its callers jal); the word at
 * 0x004567AC is junk between objects (src/us/asm/stray_004567AC.s).
 * Second-module twin of SaveGame_Build. */
#include "common.h"
#include "structs/game.h"
#include "structs/save.h"
#include "globals/game.h"
#include "globals/pak.h"
#include "globals/scene.h"

/* Build the save block: the player's profile, the multiplayer settings and the
 * player's options are copied into a freshly allocated buffer. */

/* The end of the function reads the size through a struct view (address in a
 * register, reloaded after the extraSize store); the top reads it as a scalar. */
typedef struct {
    s32 size;
} ExtraInfo;

extern s32 D_800F70A8; /* size of the extra data */

/* Struct pointee: the load of ->data is scheduled before the gSaveBufferHandle store. */
typedef struct {
    void *data;
} MemHandle;

extern char D_800C13F0[];

void CCache__Flush(s32 heap);
void CCache__Deallocate(sptr heap, MemHandle *handle);
MemHandle *CCache__Allocate(s32 heap, s32 size, s32 tag, char *name);
void memset(void *dst, s32 value, sptr size);
void func_0029E460(void *dst, void *src, s32 size);

void SaveGame_Build_2(CPlayer *player) {
    s32 size = D_800F70A8 + 0x688;
    s32 allocSize;
    SaveGame *data;

    if (gSaveBufferHandle != NULL) {
        CCache__Flush(0);
        CCache__Deallocate(0, gSaveBufferHandle);
    }
    allocSize = D_800F70A8 + 0x688;
    gSaveBufferHandle = CCache__Allocate(0, allocSize, 0x33, D_800C13F0);
    gSaveBuffer = gSaveBufferHandle->data;
    memset(gSaveBuffer, 0, allocSize);
    gSaveBuffer->player = player->m_Data;
    gSaveBuffer->settings = *(GameSettingsBlock *)&gEngineApp_m_Settings;
    data = gSaveBuffer;
    data->frames = gEngineApp_m_Settings.match.frames;
    data->seconds = gEngineApp_m_Settings.match.seconds;
    data->minutes = gEngineApp_m_Settings.match.minutes;
    data->hours = gEngineApp_m_Settings.match.hours;
    data->size = size;
    data->levelFlagsSize = ((ExtraInfo *) &D_800F70A8)->size;
    func_0029E460(data->levelFlags, gLoadSavePersistentData, ((ExtraInfo *) &D_800F70A8)->size);
}
