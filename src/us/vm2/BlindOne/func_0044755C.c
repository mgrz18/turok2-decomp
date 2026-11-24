#include "common.h"
#include "structs/vector.h"
#include "structs/boss.h"
#include "blindone_scene.h"
#include "protos/scene.h"

/* Only what this reads and writes. */


void BlindOne_PhaseMinionsInit_2(BlindOneState *arg0) {
    CVector3 *temp_v0;

    temp_v0 = arg0->script;
    arg0->toSpawn = (s32) temp_v0->y;
    arg0->maxMinions = (s32) temp_v0->z;
    CScene__MarkGeneratorsSaved(&gScene);
}
