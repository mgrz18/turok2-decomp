#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);
extern s32 WeightedTable_PickStatic(s32 id);
extern void Sound__Play(s16 sound, CVector3 pos, s32 arg2, CGameObjectInstance *source);


/* Run the current step of the third script (sounds, jumps, waits). */
s32 Primagen_RunSoundStep(CGameObjectInstance *obj) {
    s32 sound;
    s32 i;

    if (gLevelScript.script3.timer > 0.0f) {
        return 0;
    }
    if (gLevelScript.script3.steps == NULL) {
        return 0;
    }
    switch (gLevelScript.script3.steps[gLevelScript.script3.current].type) {
        case 1:
            if (gLevelScript.script3.steps[gLevelScript.script3.current].arg != -1) {
                Sound__Play(gLevelScript.script3.steps[gLevelScript.script3.current].arg, obj->header.m_vPos, 0, obj);
            }
            gLevelScript.script3.current = -1;
            return 1;
        case 2:
            sound = (s16) WeightedTable_PickStatic(gLevelScript.script3.steps[gLevelScript.script3.current].arg);
            if (sound != -1) {
                Sound__Play(sound, obj->header.m_vPos, 0, obj);
            }
            gLevelScript.script3.current = -1;
            return 1;
        case 3:
            gLevelScript.script3.steps = (BossStep3 *) gLevelScript.script3.steps[gLevelScript.script3.current].arg;
            gLevelScript.script3.cursor = -1;
            gLevelScript.script3.current = -1;
            return 1;
        case 5:
            gLevelScript.script3.current = -1;
            return 1;
        case 6:
            gLevelScript.script3.timer = gLevelScript.script3.steps[gLevelScript.script3.cursor].delay
                             + Random_Blend(0.0f, gLevelScript.script3.steps[gLevelScript.script3.cursor].delayRange) * 15.0f;
            for (i = 0; i < gLevelScript.script3.steps[gLevelScript.script3.current].arg; i++) {
                gLevelScript.script3.cursor++;
                if (gLevelScript.script3.steps[gLevelScript.script3.cursor].type == 4 && i < gLevelScript.script3.steps[gLevelScript.script3.current].arg - 1) {
                    gLevelScript.script3.cursor = 0;
                }
                gLevelScript.script3.current = gLevelScript.script3.cursor;
            }
            if (gLevelScript.script3.steps[gLevelScript.script3.cursor].type == 4) {
                gLevelScript.script3.cursor = -1;
                gLevelScript.script3.current = -1;
                gLevelScript.script3.steps = NULL;
                return 1;
            }
            return 0;
        case 4:
            break;
    }
    return 0;
}
