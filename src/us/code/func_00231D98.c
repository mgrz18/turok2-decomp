#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002838F4.c, func_002838F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
#include "structs/particle.h"
#endif
typedef struct B19Def B19Def;
typedef struct B19Obj B19Obj;
struct B19Def {
              u8 pad0[0x6];
              s8 b6;
              s8 b7;
              u8 pad8[0x4];
              s16 sC;
};
#ifdef TARGET_PORT
struct B19Obj {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_B19Obj(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_id[T2_HOFF_B19Obj(header.m_nObjType)]; u16 id; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_B19Obj(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_flags[T2_HOFF_B19Obj(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_unk118[T2_HOFF_B19Obj(m_pDLight)]; s32 unk118; }; /* N64 0x118 */
    };
};
#else
struct B19Obj {
    u8 pad0[0x2];
    u16 id;          /* 0x2 */
    CVector3 pos;     /* 0x4 */
    u8 pad10[0x30];
    u32 flags;       /* 0x40 */
    u8 pad44[0xD4];
    s32 unk118;      /* 0x118 */
};
#endif
extern u8 gEngineApp_m_SwooshSystem[];
#ifdef TARGET_PORT
#include "structs/particle.h"
#include "globals/scene.h"
#define D_8010FF34 (&gScene.m_DLightSet)
#else
extern u8 D_8010FF34[];
#endif
void CParticle__Attach(B19Obj *, sptr, sptr);
void Trail_AddNode(void *, B19Obj *, s32);
#ifdef TARGET_PORT
void *DLight_Spawn(void *, s32);
#else
sptr DLight_Spawn(void *, s32);
#endif
void CParticle__StartLoopSound(B19Obj *, s32);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sound__Play Sound__Play_f3
#endif
void Sound__Play(s16, f32, f32, f32, s32, s32);
void CParticle__StartEffects(B19Obj *o, B19Def *def) {
    s32 t;
    CParticle__Attach(o, 0, 0);
    if (def->b6 != -1) Trail_AddNode(gEngineApp_m_SwooshSystem, o, def->b6);
#ifdef TARGET_PORT
    /* the light pointer-wide, in the host CParticle's member (the view has the N64 offset) */
    if (def->b7 != -1) ((CParticle *) o)->m_pDLight = DLight_Spawn(D_8010FF34, def->b7);
    else ((CParticle *) o)->m_pDLight = NULL;
#else
    if (def->b7 != -1) o->unk118 = DLight_Spawn(D_8010FF34, def->b7);
    else o->unk118 = 0;
#endif
    if (!(o->flags & 0x200000) && (t = def->sC) != 0) {
        switch (o->id) {
        case 0x22:
        case 0x5F:
        case 0x60:
            CParticle__StartLoopSound(o, t);
            break;
        default:
            Sound__Play(t, o->pos.x, o->pos.y, o->pos.z, 0, -1);
            break;
        }
    }
}
