#include "common.h"
#include "protos/iset.h"
#include "structs/world.h"

#ifdef TARGET_PORT /* the scene (CScene, structs/world.h) at its host layout: m_pIntelligence */
typedef struct {
    u8 pad0[__builtin_offsetof(CScene, m_pIntelligence)];
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x64];
#endif
    void *file;
} Resource;


#define arg0 ((Resource *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the view reads it */
sptr CScene__GetIntelligence(CScene *scene, s32 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    return CISet__GetBlock(arg0->file, arg1);
}
#undef arg0
