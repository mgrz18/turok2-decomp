/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C.CPP, func_802846C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/particle.h"

/* Finds the particle of owner arg1 (and of template arg2, when given) in the template's active list. */
void *CParticleSystem__FindOwned(CParticleSystem *arg0, s32 arg1, CROMParticleEffect *arg2) {
    CParticle *record;
    /* index[array] order matches the original addu operand order */
    record = arg2->m_pParams->m_nList[arg0->m_ActiveList].m_pHead;
    if (arg2 != 0) {
        if (record != 0) {
            do {
                if ((s32) record->m_pOwnerID == arg1) {
                    if (record->m_pEffect == arg2) {
                        return record;
                    }
                }
                record = record->m_pNext;
            } while (record != 0);
        }
    } else if (record != 0) {
        do {
            if ((s32) record->m_pOwnerID == arg1) {
                return record;
            }
            record = record->m_pNext;
        } while (record != 0);
    }
    return 0;
}
