#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_4.CPP, func_8028C424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "protos/scene.h"
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
s32 CScene__AreEventsDone(char *object, s32 type) {
    s32 index;
    CSceneRecord *record;
    u8 *flags;
    index = ((CScene *)object)->m_nRecords;
    record = ((CScene *)object)->m_pRecords;
    index--;
    if (index != -1) {
        flags = &record->m_Flags;
        do {
            if (flags[1] == type &&
                (*flags & 0x40) == 0) {
                return 0;
            }
            index--;
            flags += sizeof(CSceneRecord);
        } while (index != -1);
    }
    index = ((CScene *)object)->m_nAltRecords;
    record = ((CScene *)object)->m_pAltRecords;
    index--;
    if (index != -1) {
        do {
            if (record->m_Key == type &&
                (record->m_Flags & 0x40) == 0) {
                return 0;
            }
            index--;
            record++;
        } while (index != -1);
    }
    return 1;
}
