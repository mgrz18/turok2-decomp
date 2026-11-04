typedef signed char s8;
typedef float f32;

typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} Vec3;

typedef struct {
    /* 0x0 */ f32 nx;
    /* 0x4 */ f32 ny;
    /* 0x8 */ f32 nz;
    /* 0xC */ f32 d;
} Plane;

extern f32 D_800A8090;

/* Where the segment a->b crosses the plane, as a fraction of the way. */
f32 func_0026F400(Plane *arg0, Vec3 *arg1, Vec3 *arg2)
{
  f32 temp_f1;
  f32 new_var;
  f32 temp_f1_2;
  f32 temp_f3;
  f32 temp_f4;
  f32 temp_f5;
  temp_f3 = arg0->nx;
  temp_f4 = arg0->ny;
  temp_f1_2 = arg0->nz;
  temp_f5 = ((temp_f3 * arg1->x) + (temp_f4 * arg1->y)) + (temp_f1_2 * arg1->z);
  new_var = arg2->y;
  temp_f1 = ((temp_f3 * arg2->x) + (temp_f4 * new_var)) + (temp_f1_2 * arg2->z);
  if (temp_f1 != temp_f5)
  {
    if (arg1)
    {
      return (arg0->d - temp_f5) / (temp_f1 - temp_f5);
    }
    else
    {
      return (arg0->d - temp_f5) / (temp_f1 - temp_f5);
    }
  }
  return D_800A8090;
}
