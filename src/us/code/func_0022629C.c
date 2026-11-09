#include "common.h"
#include "m2c_macros.h"
#include "structs/world.h"
#include "structs/cache.h"
#include "protos/iset.h"
#include "protos/scene.h"

s32 CCache__Deallocate(s32, sptr);
sptr CCache__LoadPersistantIndex(s32, sptr, sptr, sptr);
sptr CCache__LoadPersistantSubBlock(s32, sptr, s32, s32, sptr, sptr, sptr);

void *func_0029E460(void *, void *, s32);           /* memcpy */
extern M2C_UNK D_800A5EC0;
extern M2C_UNK D_800A5FD0;

/* Copies the level's name (sub-block 8 of its level file, a handle: data then size) into
 * arg2, at most arg3 bytes. The level file stays loaded when it is the current level. */
void CScene__GetLevelName(struct CScene *arg0, s32 arg1, void *arg2, s32 arg3) {
    sptr sp20;
    sptr temp_s2;
    s32 temp_v0;
    s32 var_a2;
    CMemEntry *temp_v0_2;

    temp_v0 = CISet__GetROMAddress((sptr) arg0->m_pLevels, arg0->m_LevelsKey, arg1);
    temp_s2 = CCache__LoadPersistantIndex(0, (sptr) &sp20, temp_v0, (sptr) &D_800A5EC0);
    temp_v0_2 = (CMemEntry *) CCache__LoadPersistantSubBlock(0, sp20, 8, temp_v0, (sptr) arg0, 0, (sptr) &D_800A5FD0);
    var_a2 = temp_v0_2->size;
    if (arg3 < var_a2) {
        var_a2 = arg3;
    }
    func_0029E460(arg2, temp_v0_2->data, var_a2);
    CCache__Deallocate(0, (sptr) temp_v0_2);
    if (arg0->m_nLevel != arg1) {
        CCache__Deallocate(0, temp_s2);
    }
}
