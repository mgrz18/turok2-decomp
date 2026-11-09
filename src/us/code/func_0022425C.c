#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B2F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "protos/iset.h"
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
void *CScene__GetRegionInfo(CScene *arg0, u16 *arg1) {
    s32 base;
    s32 idx;
    if (arg1 == 0) {
        return 0;
    }
    base = ((char *)CISet__GetBlock(arg0->m_pRegionData, 0)) + 8;
    idx = *arg1;
    return (void *)(base + idx * 0x64);
}
