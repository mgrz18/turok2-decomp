#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "structs/region.h"
#include "protos/iset.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__DecompressCollision(void *, void *);
sptr CScene__GetRegion(void *, s32);
u32 CScene__GetRegionIndex(struct CScene *, sptr);
void CScene__SetLevelBit(void *, s32, s32);
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
u32 CScene__GetRegionIndex(CScene *arg0, sptr arg1) {
    sptr offset;
    sptr result;
    if (arg1 == 0) {
        return -1;
    }
    offset = ((char *)CISet__GetBlock(arg0->m_pRegionData, 2)) + 8;
    result = arg1 - offset;
    /* the host CGameRegion stride (sizeof(CGameRegion), three pointer pairs), as
     * CScene__GetRegion indexes it, not the N64's 0x20 bytes (>> 5). Round 9 fixed it twice:
     * x1: the index came out too big, Automap_RevealRegion's CScene__SetLevelBit set map bits
     * past the level's bit section into the next level's block of gGamePersistentData (the
     * warp 55 -> 54 crashed in CScene__DecompressCollision); x2: a cinema that saved the
     * player's region (Cinema_Start) restored him into another region or past the table (the
     * camera's region read floats and crashed after the Mother's phase-2 cinema). Unsigned,
     * like the N64's shift. */
    return (u32) ((uptr) result / sizeof(CGameRegion));
}
