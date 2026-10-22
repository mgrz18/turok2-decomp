/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00216F44.c, func_80216F44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern f32 func_0021153C(f32 arg0);
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
struct func_80216F44_S1;
typedef struct func_80216F44_S1 func_80216F44_S1;
struct func_80216F44_S1;
struct func_80216F44_S1 {
    char pad0[0x4];
    f32 unk4;   /* pos.x */
    char pad8[0xC - 0x4 - sizeof(f32)];
    f32 unkC;   /* pos.z */
    char pad10[0x50 - 0xC - sizeof(f32)];
    f32 unk50;  /* yaw */
};
extern f32 func_00298470(f32);
f32 func_00246310(void *actor, Vec3 point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;
    dx = point.x - ((func_80216F44_S1 *)(actor))->unk4;
    dz = point.z - ((func_80216F44_S1 *)(actor))->unkC;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(((func_80216F44_S1 *)(actor))->unk50);
    } while (0);
    c = -func_002974C0(((func_80216F44_S1 *)(actor))->unk50);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot)))<((-1.0f))?((-1.0f)):(((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = func_0021153C(dot);
    if (dx * c + dz * s > 0.0f) {
        return angle;
    }
    return -angle;
}
