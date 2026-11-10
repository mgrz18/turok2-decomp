#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"

typedef struct Sphere {
    /* 0x00 */ CVector3 center;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 radiusSq;
} Sphere;

f32 func_00268460(sptr);
void Sphere_Set(Sphere *sphere, CVector3 *center, f32 radius);
s32 Cylinder_ContainsPoint(Sphere *sphere, CVector3 *point);
s32 Circle_SegmentIntersects(Sphere *sphere, CVector3 *a, CVector3 *b);
s32 Sweep_CrossRegionEdge(s32 *mover, CGameRegion *region, s32 edge, CVector3 *pos);

/* Whether a mover at pos fits in region: for each edge not shared with prev
 * or goal and not passable (Sweep_CrossRegionEdge), the mover's circle must not
 * touch either corner or the edge itself. */
s32 Path_FitsInRegion(s32 *mover, CGameRegion *region, CVector3 *pos, CGameRegion *prev, CGameRegion *goal) {
    Sphere sphere;
    CVector3 *a;
    CVector3 *b;
    s32 i;

    if (region == prev || region == goal) {
        return 1;
    }
    Sphere_Set(&sphere, pos, func_00268460(*mover) * 0.5f);
    for (i = 0; i < 3; i++) {
        if (region->m_pNeighbours[i] == prev) {
            continue;
        }
        if (region->m_pNeighbours[i] == goal) {
            continue;
        }
        if (Sweep_CrossRegionEdge(mover, region, i, pos) != 0) {
            continue;
        }
        a = region->m_pVertices[i];
        b = region->m_pVertices[i == 2 ? 0 : i + 1];
        if (Cylinder_ContainsPoint(&sphere, a) || Cylinder_ContainsPoint(&sphere, b)) {
            return 0;
        }
        if (Circle_SegmentIntersects(&sphere, a, b)) {
            return 0;
        }
    }
    return 1;
}
