#include "common.h"
#include "structs/collision.h"

s32 CCollisionSetup__Construct(s32 *);                   /* extern */
extern s32 gCollisionSetupTable;                      /* the CCollisionSetup_t table */
extern CCollisionResults *gCollisionResultsP;
extern CCollisionResults gCollisionResults;

void CCollisionInfo__Reset(void) {
    gCollisionResultsP = &gCollisionResults;
    CCollisionSetup__Construct(&gCollisionSetupTable);
}
