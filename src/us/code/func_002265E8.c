/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_8.CPP, func_8028D74C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "protos/scene.h"


/* Turok 2 layouts (Rage Wars offsets in comments). */

typedef struct Container {
    u8 pad0[0xE0];
    CGameObjectInstance *actors;        /* 0xE0 (0x138) */
    s32 unkE4;
    s32 actorCount;            /* 0xE8 (0x140) */
} Container;

/* Index of arg1 in the scene's actor array, or -1. */
s32 CScene__GetInstanceIndex(void *arg0, void *arg1) {
    Container *scene = arg0;
    CGameObjectInstance *actor = arg1;
    void *base;
    if (actor->m_dwFlags & 0x80000) {
        return -1;
    }
    base = scene->actors;
    if (arg1 >= base &&
        (char *)arg1 <= (char *)base + ((u32)scene->actorCount * sizeof(CGameObjectInstance) - sizeof(CGameObjectInstance))) {
        return ((uptr)arg1 - (uptr)base) / sizeof(CGameObjectInstance);
    }
    return -1;
}
