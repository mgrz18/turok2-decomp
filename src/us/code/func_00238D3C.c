#include "structs/vector.h"
#include "globals/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160.CPP, func_802681A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "globals/rumble.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
#endif
struct Triple;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_class): the position is a float struct on the host.
 * MIPS passes the Rage Wars Triple (three s32) and a CVector3 in the same registers; x86-64
 * and arm64 do not (integer vs SSE/FP registers), so Triple means three floats here. */
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif

struct func_8024E8F0_S1;
struct func_8024E8F0_S1 {
    u8 unk0;
    char pad0[0xD4 - 0x0 - sizeof(u8)];
    s32 unkD4;
};
struct func_8024E8F0_S1;
typedef struct func_8024E8F0_S1 func_8024E8F0_S1;
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct Triple;
struct Triple;
struct Triple;
struct Triple;
struct Inner;
typedef struct Inner Inner;
struct Params_func_802681A8_de;
struct Params_func_802681A8_de {
    s32 value;
    s16 angle;
    u8 scale;
    u8 pad;
};
struct Context_func_802681A8_de;
typedef struct Context_func_802681A8_de Context_func_802681A8_de;
struct Params_func_802681A8_de;
typedef struct Params_func_802681A8_de Params_func_802681A8_de;
struct Inner;
struct Inner {
    char pad[0xA50];
    char *resource;
};
struct Context_func_802681A8_de;
struct Inner;
struct Context_func_802681A8_de {
    char pad[0x1A8];
    struct Inner *inner;
};
struct Triple;
struct Triple;
extern void CScene__StartRumbles(void *, void *, s32, Triple, f32, f32);
void ImpactEvent_Rumble(func_8024E8F0_S1 *arg0, Context_func_802681A8_de *arg1, s32 arg2, Triple arg3, Params_func_802681A8_de arg6) {
    void *resource;
#ifdef TARGET_PORT
    /* Host members (STABILITY.md section 7): the N64 view read the actor's flags at 0xD4, its
     * player (CAI m_pParent, actor 0x1A8), the player's controller (0xA50) and the rumble pool
     * in it (0x140) at N64 offsets. */
    CGameObjectInstance *hitActor = (CGameObjectInstance *) arg0;
    if (hitActor->header.m_Type == 1 && (hitActor->m_dwFlags & 0x300000)) {
        CPlayer *player = (CPlayer *) ((CGameObjectInstance *) arg1)->m_AI.m_pParent;
        resource = player->m_ContState.m_pContState->rumblePool;
#else
    if ((arg0->unk0 == 1) && (arg0->unkD4 & 0x300000)) {
        resource = arg1->inner->resource + 0x140;
#endif
        arg3.x = 0;
        arg3.y = 0;
        arg3.z = 0;
    } else {
        resource = &gRumblePool;
    }
    CScene__StartRumbles(&gScene, resource, arg6.value, arg3,
                  arg6.angle * 10.24f, arg6.scale * 0.003921569f);
}
