#include "common.h"
#include "structs/world.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
#ifdef TARGET_PORT
void *CScene__GetSwoosh(struct CScene *, s32); /* host: returns the block (docs/port/ABI.md) */
#else
void CScene__GetSwoosh(struct CScene *);
#endif

#ifdef TARGET_PORT
#include "protos/iset.h" /* CISet__GetBlock returns a pointer: implicit int truncated it (--high-memory) */
/* ABI (docs/port/ABI.md, abicheck void_used + missing_args): the caller (Trail_AddNode) passes the
 * swoosh type in a1 and uses the block address; on MIPS a1 passes straight through to
 * CISet__GetBlock and its v0 is returned. */
void *CScene__GetSwoosh(CScene *arg0, s32 index) {
    return CISet__GetBlock(arg0->m_pSwooshes, index);
}
#else

void CScene__GetSwoosh(CScene *arg0) {
    CISet__GetBlock((sptr)arg0->m_pSwooshes);
}
#endif
