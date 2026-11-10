#include "common.h"
#include "globals/scene.h"
#include "protos/cache.h"

/* The record this spawns from: an id at 0x24, extra spawn flags at 0x80 and its own flags at 0x88 (0x40: disabled). */
#ifdef TARGET_PORT /* CStaticObject's host layout (structs/level_actor.h): id = m_pOwner */
#include "structs/level_actor.h"
typedef struct {
    u8 pad0[0x30];
    void **id;
    f32 pad28[16];
    u8 pad68[0x18];
    s32 spawnFlags;
    u8 pad84[4];
    u16 flags;
} SpawnSource;
T2_STATIC_OBJECT_CHECK(SpawnSource, pad28, flags);
_Static_assert(__builtin_offsetof(SpawnSource, id) == __builtin_offsetof(CStaticObject, m_pOwner), "id");
_Static_assert(__builtin_offsetof(SpawnSource, spawnFlags) == __builtin_offsetof(CStaticObject, m_LitKey), "spawnFlags");
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 id;
    /* 0x28 */ u8 pad28[0x80 - 0x28];
    /* 0x80 */ s32 spawnFlags;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 flags;
} SpawnSource;
#endif

s32 Model_RequestTextures(s32);                         /* extern */
s32 CStaticObject__LoadGeometry();                                /* extern */
extern s32 D_800A57F8;
extern s32 CStaticObject__BuildLitVerticesCallback;

void CStaticObject__RequestLitVertices(SpawnSource *arg0) {
    sptr temp_v0;
    sptr temp_v0_2;

    if (!(arg0->flags & 0x40)) {
#ifdef TARGET_PORT /* the ROM passes arg0 through in $a0 */
        temp_v0 = (sptr) CStaticObject__LoadGeometry(arg0);
#else
        temp_v0 = CStaticObject__LoadGeometry();
#endif
        if (temp_v0 != 0) {
            Model_RequestTextures(temp_v0);
            temp_v0_2 = CCache__RequestDecompression(0, arg0->spawnFlags | gLitVerticesKey, temp_v0, arg0->id, 8, arg0, &CStaticObject__BuildLitVerticesCallback, &D_800A57F8, 0);
            if (temp_v0_2 != 0) {
                CCache__Unlock(0, temp_v0_2);
            }
            CCache__Unlock(0, temp_v0);
        }
    }
}
