#include "common.h"
#include "structs/collision.h"
#include "globals/collision.h"

s32 CCollisionSetup__Construct(CCollisionSetup *);                   /* extern */

void CCollisionInfo__Reset(void) {
    gCollisionResultsP = &gCollisionResults;
    CCollisionSetup__Construct(gCollisionSetupTable);
}
