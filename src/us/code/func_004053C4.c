#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);
#ifdef TARGET_PORT
extern s16 WeightedTable_PickStatic(sptr id); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
extern s32 WeightedTable_PickStatic(sptr id);
#endif
extern void Sound__Play(s16 sound, CVector3 pos, sptr arg2, CGameObjectInstance *source);
#ifdef TARGET_PORT
/* A step's arg is an N64 data address for steps 2 (a sound table) and 3 (the next script):
 * BossStep3.arg is an s32 in the data, so gendata keeps the N64 word; the host pointer comes
 * from the data symbol table (port/gen/relocs.c). */
void *t2_n64_to_host(u32 addr);
#define BOSS_STEP_PTR(v) ((sptr) t2_n64_to_host((u32) (v)))
/* the sound pick tables the type-2 steps name (round 10 y2): only an s32 in the data reaches them,
 * so nothing emitted them and t2_n64_to_host returned NULL (the first type-2 step crashed in
 * WeightedTable_PickStatic once the Primagen left its first stage). Referenced here, gendata
 * emits them as Pick tables (host byte order) and the symbol table maps their N64 addresses. */
#include "structs/pick.h"
extern Pick D_004315C8[], D_004315DC[], D_00431608[], D_00431618[];
Pick *const t2_sPrimagenSoundPicks[] = { D_004315C8, D_004315DC, D_00431608, D_00431618 };
#else
#define BOSS_STEP_PTR(v) (v)
#endif


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
            sound = (s16) WeightedTable_PickStatic(BOSS_STEP_PTR(gLevelScript.script3.steps[gLevelScript.script3.current].arg));
            if (sound != -1) {
                Sound__Play(sound, obj->header.m_vPos, 0, obj);
            }
            gLevelScript.script3.current = -1;
            return 1;
        case 3:
            gLevelScript.script3.steps = (BossStep3 *) BOSS_STEP_PTR(gLevelScript.script3.steps[gLevelScript.script3.current].arg);
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
