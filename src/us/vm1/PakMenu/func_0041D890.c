#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/save.h"
#include "globals/game.h"
#include "globals/pak.h"
#include "globals/scene.h"


extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */
extern void CCache__Deallocate(sptr, void *);
extern void **CCache__Allocate(s32, s32, s32, char *);
extern void memset(void *dst, s32 value, sptr size);
extern void func_0029E460(char *dst, char *src, s32 n);

static __inline__ s32 SaveFileSize(void) {
    return gScene.unk_30 + 0x688;
}

/* Snapshots the player and the multiplayer setup into a freshly allocated buffer. */
void SaveGame_Build(CPlayer *p) {
    s32 size = SaveFileSize();
    SaveGame *save;
    s32 allocSize;
    s32 *handle;
    SaveGame *buf;

    if (gSaveBufferHandle != NULL) {
        CCache__Flush_Arg(0);
        CCache__Deallocate(0, gSaveBufferHandle);
    }
    allocSize = SaveFileSize();
    handle = CCache__Allocate(0, allocSize, 0x33, "Single Save Block");
    buf = *(SaveGame **) handle; /* the block's data pointer (CMemEntry.data), pointer-wide on the host */
    gSaveBufferHandle = handle;
    gSaveBuffer = buf;
    memset(gSaveBuffer, 0, allocSize);
    gSaveBuffer->player = p->m_Data;
    gSaveBuffer->settings = *(GameSettingsBlock *)&gEngineApp_m_Settings;
    save = gSaveBuffer;
    save->frames = gEngineApp_m_Settings.match.frames;
    save->seconds = gEngineApp_m_Settings.match.seconds;
    save->minutes = gEngineApp_m_Settings.match.minutes;
    save->hours = gEngineApp_m_Settings.match.hours;
    /* gScene.unk_30 is a struct member, so it is reloaded after the store into the header. */
    save->size = size;
    save->levelFlagsSize = gScene.unk_30;
    func_0029E460((char *)save->levelFlags, gLoadSavePersistentData, gScene.unk_30);
}
