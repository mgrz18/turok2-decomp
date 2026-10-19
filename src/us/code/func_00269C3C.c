/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_8027525C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A7F68;
extern int D_800F2000;
extern int D_800B2600;
extern int D_800F2000;
extern int D_800B2600;
extern void func_0020EFDC(Vec3 *, Vec3 *, Vec3 *);
f32 func_00269C3C(Node75 *node, f32 x, f32 z) {
    Vec3 normal;
    Vec3 point;
    Vec3 a;
    Vec3 b;
    char *p;
    char *q;
    if (node == 0) {
        return 0.0f;
    }
    if ((int)node != D_800B2600) {
        p = node->cur;
        q = node->prev;
        a.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = node->cur;
        q = node->prev;
        a.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = node->cur;
        q = node->prev;
        a.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        p = node->next;
        q = node->cur;
        b.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = node->next;
        q = node->cur;
        b.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = node->next;
        q = node->cur;
        b.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        func_0020EFDC((Vec3 *)&D_800F2000, &b, &a);
    }
    normal = *(Vec3 *)&D_800F2000;
    D_800B2600 = (int)node;
    if (normal.y == 0.0f) {
        return (((CollisionBuildVertex *)node->prev)->ceiling +
                ((CollisionBuildVertex *)node->cur)->ceiling +
                ((CollisionBuildVertex *)node->next)->ceiling) * D_800A7F68;
    }
    p = node->prev;
    point.x = ((CollisionBuildVertex *)p)->x;
    p = node->prev;
    point.y = ((CollisionBuildVertex *)p)->ceiling;
    p = node->prev;
    point.z = ((CollisionBuildVertex *)p)->z;
    return (((point.z - z) * normal.z) +
            ((point.x - x) * normal.x) + (point.y * normal.y)) /
           normal.y;
}
