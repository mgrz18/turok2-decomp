#include "common.h"
#include "protos/iset.h"
#include "structs/world.h"

#ifdef TARGET_PORT /* the scene (CScene, structs/world.h) at its host layout: m_pIntelligence */
#define RESOURCE_FILE_AT __builtin_offsetof(CScene, m_pIntelligence)
#else
#define RESOURCE_FILE_AT 0x64
#endif
/* (one conditional block of its own: tools/merge_module.py reads the definition below as a function) */
typedef struct {
    /* 0x00 */ u8 pad0[RESOURCE_FILE_AT];
    void *file;
} Resource;


/* the definition takes the callers' type (one prototype for the merged object); the view reads it */
sptr CScene__GetIntelligence(CScene *scene, s32 arg1) {
#define arg0 ((Resource *) scene)
    if (arg1 == -1) {
        return 0;
    }
    return CISet__GetBlock(arg0->file, arg1);
#undef arg0
}
