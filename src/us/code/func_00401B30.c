#include "common.h"

typedef struct {
    s32 v[3];
} Triple;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 a;
    /* 0x10 */ s32 b;
    /* 0x14 */ Triple c;
} Obj00401B30;

void Automap_RevealAroundPlayer(Obj00401B30 *obj);

#ifdef TARGET_PORT
/* The port: the caller's types (CPlayerPool__Update: the region, the facing as f32 and the
 * position as a CVector3 by value; as s32 and the Rage Wars Triple they arrived in other
 * registers on x86-64). The region goes to CAutomap.m_pHostRegion, the N64 word keeps
 * whether there is one (Automap_Draw tests it). */
#include "structs/game.h"
#include "structs/vector.h"
void Automap_SetPlayer(Obj00401B30 *obj, struct CGameRegion *a, f32 b, CVector3 c) {
    obj->a = a != NULL;
    *(f32 *) &obj->b = b;
    *(CVector3 *) &obj->c = c;
    ((CAutomap *) obj)->m_pHostRegion = a;
    Automap_RevealAroundPlayer(obj);
}
#else

void Automap_SetPlayer(Obj00401B30 *obj, sptr a, s32 b, Triple c) {
    obj->a = a;
    obj->b = b;
    obj->c = c;
    Automap_RevealAroundPlayer(obj);
}
#endif
