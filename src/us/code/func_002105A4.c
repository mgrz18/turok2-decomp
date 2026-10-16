/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_802736D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern void func_002105A4(void *matrix, f32 angle);
struct Mtx;
struct Mtx {
    Vec3 right;
    f32 m03;
    Vec3 up;
    f32 m13;
    Vec3 forward;
    f32 m23;
    f32 m30;
    f32 m31;
    f32 m32;
    f32 m33;
};
struct Mtx;
typedef struct Mtx Mtx;
void func_002105A4(void *arg0, f32 arg1)
{
    extern f32 D_800A561C[];
  struct Mtx *m = (char *) arg0;
  f32 sin_v;
  f32 cos_v;
  f32 zero;
  f32 identity;
  zero = 0;
  sin_v = func_002982F0(arg1);
  identity = D_800A561C[1];
  m->forward.x = sin_v;
  m->right.y = zero;
  m->right.z = -sin_v;
  m->up.x = zero;
  m->up.z = zero;
  m->forward.y = zero;
  m->m32 = zero;
  m->m31 = zero;
  m->m30 = zero;
  m->m23 = zero;
  m->m13 = zero;
  m->m03 = zero;
  m->m33 = identity;
  m->up.y = identity;
  cos_v = func_002974C0(arg1);
  m->forward.z = cos_v;
  m->right.x = cos_v;
}
