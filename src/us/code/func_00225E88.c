#include "common.h"
#include "protos/iset.h"

#ifdef TARGET_PORT /* the scene (CScene, structs/world.h) at its host layout: m_pIntelligence */
#include "structs/world.h"
typedef struct {
    u8 pad0[__builtin_offsetof(CScene, m_pIntelligence)];
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x64];
#endif
#ifdef TARGET_PORT
    void *file;
#else
    /* 0x64 */ s32 file;
#endif
} Resource;


s32 CScene__GetIntelligence(Resource *arg0, s32 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    return CISet__GetBlock(arg0->file, arg1);
}
