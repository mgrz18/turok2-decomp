/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C.CPP, func_802750B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Node75;
typedef struct Node75 Node75;
struct Node75;
struct Node75 {
    u16 field0;
    u16 flags;
    CVector3 *prev;
    CVector3 *cur;
    CVector3 *next;
    u8 reserved10[0x10];
};
extern float D_800A7F60;
extern float D_800A7F64;
typedef struct CollisionBuildVertex {
    f32 x;
    f32 y;
    f32 z;
    f32 ceiling;
} CollisionBuildVertex;
extern int D_800F2000;
extern int D_800F2010;
extern int D_800B2604;
extern int D_800B2600;
extern void func_0020F040(f32 *arg0);
struct Node75;
extern int D_800F2000;
extern int D_800F2010;
extern int D_800B2600;
extern int D_800B2604;
extern void func_0020EFDC(CVector3 *, CVector3 *, CVector3 *);
static __inline__ CVector3 *cross_edges(CVector3 *out, Node75 *node) {
    CVector3 a;
    CVector3 b;
    char *p;
    char *q;
    if (node == 0) {
        ((CVector3 *)&D_800F2000)->x = 0;
        ((CVector3 *)&D_800F2000)->z = 0;
        ((CVector3 *)&D_800F2000)->y = D_800A7F64;
    } else if ((int)node != D_800B2600) {
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
        func_0020EFDC((CVector3 *)&D_800F2000, &b, &a);
    }
    D_800B2600 = (int)node;
    *out = *(CVector3 *)&D_800F2000;
    return out;
}
CVector3 *func_00269A90(CVector3 *out, Node75 *node) {
    if (node == 0) {
        ((CVector3 *)&D_800F2010)->x = 0;
        ((CVector3 *)&D_800F2010)->z = 0;
        ((CVector3 *)&D_800F2010)->y = D_800A7F60;
    } else if ((int)node != D_800B2604) {
        cross_edges((CVector3 *)&D_800F2010, node);
        func_0020F040((CVector3 *)&D_800F2010);
    }
    *out = *(CVector3 *)&D_800F2010;
    D_800B2604 = (int)node;
    return out;
}
