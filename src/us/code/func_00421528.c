/* gScene_m_PreviewOptions is declared as an array so its store keeps its place after the save buffer reads (a scalar is assumed not to alias them and is hoisted). */
#include "common.h"
#include "structs/player.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/save.h"
#include "globals/game.h"

extern CScene gScene;
extern s32 gScene_m_PreviewOptions[];
extern u8 gLoadSavePersistentData[];

void SaveGame_Restore(CPlayer *p) {
    CScene *level = &gScene;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;

    keep = gEngineApp_m_Settings.unk180;
    p->m_Data = gLoadBuffer->player;
    *(GameSettingsBlock *)&gEngineApp_m_Settings = gLoadBuffer->settings;
    gEngineApp_m_Settings.match.frames = gLoadBuffer->frames;
    gEngineApp_m_Settings.match.seconds = gLoadBuffer->seconds;
    gEngineApp_m_Settings.match.minutes = gLoadBuffer->minutes;
    gEngineApp_m_Settings.match.hours = gLoadBuffer->hours;
    gScene_m_PreviewOptions[0] = gEngineApp_m_Settings.unlockedCheats;
    gEngineApp_m_Settings.unk180 = keep;
    src = gLoadBuffer->levelFlags;
    dst = gLoadSavePersistentData;
    for (i = 0; i < level->unk_30; i++) {
        *dst++ = *src++;
    }
}
