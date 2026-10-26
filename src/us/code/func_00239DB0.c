#include "common.h"
#include "structs/game_object.h"

/* A navigation region: a triangle with its three corners and the regions
 * across each edge. */
typedef struct Region {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CVector3 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

typedef struct Sphere {
    /* 0x00 */ CVector3 center;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 radiusSq;
} Sphere;

f32 func_00268460(s32);
void func_0023CE94(Sphere *sphere, CVector3 *center, f32 radius);
s32 func_0023CEC0(Sphere *sphere, CVector3 *point);
s32 func_0023CF04(Sphere *sphere, CVector3 *a, CVector3 *b);
s32 func_0023B664(s32 *mover, Region *region, s32 edge, CVector3 *pos);

/* Whether a mover at pos fits in region: for each edge not shared with prev
 * or goal and not passable (func_0023B664), the mover's circle must not
 * touch either corner or the edge itself. */
s32 func_00239DB0(s32 *mover, Region *region, CVector3 *pos, Region *prev, Region *goal) {
    Sphere sphere;
    CVector3 *a;
    CVector3 *b;
    s32 i;

    if (region == prev || region == goal) {
        return 1;
    }
    func_0023CE94(&sphere, pos, func_00268460(*mover) * 0.5f);
    for (i = 0; i < 3; i++) {
        if (region->neighbour[i] == prev) {
            continue;
        }
        if (region->neighbour[i] == goal) {
            continue;
        }
        if (func_0023B664(mover, region, i, pos) != 0) {
            continue;
        }
        a = region->corner[i];
        b = region->corner[i == 2 ? 0 : i + 1];
        if (func_0023CEC0(&sphere, a) || func_0023CEC0(&sphere, b)) {
            return 0;
        }
        if (func_0023CF04(&sphere, a, b)) {
            return 0;
        }
    }
    return 1;
}
