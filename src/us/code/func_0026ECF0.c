/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00285C48.c, func_00285C48.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
typedef struct B19Spawn B19Spawn;
typedef struct B19PNode B19PNode;
typedef struct B19Pool B19Pool;
typedef struct B19Vec3 B19Vec3;
struct B19PNode {
              u8 pad0[0x4];
              B19PNode * next;
};
struct B19Pool {
              u8 pad0[0x14];
               B19PNode * head;
               u8 pad18[0x10];
               s32 count;
};
struct B19Vec3 {
              f32 x;
              f32 y;
              f32 z;
};
struct B19Spawn {
              u8 pad0[0x4];
              B19Spawn * next;
              void * src;
              B19Vec3 pos;
               f32 f18;
               f32 f1C;
               s32 h20;
               u8 pad24[0x8];
               s32 h2C;
               u8 pad30[0x8];
               B19Spawn ** link;
};
u32 func_0029DFF0(void);
void func_0029E010(u32);
s32 func_0026EE40(void *);
void CList__Remove(void *, void *);
void CList__AddTail(void *, void *);
void func_0026ECF0(B19Pool *pool) {
    u32 mask = func_0029DFF0();
    B19Spawn *n;
    B19Spawn *next;
    s32 r;
    for (n = (B19Spawn *)pool->head; n != 0; n = next) {
        next = *(B19Spawn **)((u8 *)n + 4);
        r = 0;
        if (func_0026EE40((u8 *)n + 0x20) || func_0026EE40((u8 *)n + 0x2C)) r = 1;
        if (r) {
            if (n->link != 0) *n->link = 0;
            CList__Remove(&pool->head, n);
            CList__AddTail(pool, n);
        }
    }
    func_0029E010(mask);
}
