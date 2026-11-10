#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"


void Generator_Step(sptr arg0, CAI *ai) {
    s32 (*temp_v0_2)(sptr, void *);
    CAIModeInfo *temp_v0;

    if ((CScene__GetInstanceSavedBit(&gScene, (void *) arg0) ^ 1) == 0) {
        temp_v0 = ai->m_pModeCurrent;
        if (temp_v0 != NULL) {
            temp_v0_2 = temp_v0->m_pfCallback;
            if (temp_v0_2 != NULL) {
                temp_v0_2(arg0, ai);
            }
        }
    }
}
