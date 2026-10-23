/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4_22.CPP, func_80217290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
typedef struct CVector3 { f32 x, y, z; } CVector3;
/* Turok 2: actor position at 0x4 (Rage Wars 0x8). */
typedef struct LevelActor { char pad0[4]; CVector3 position; } LevelActor;
/* Squared horizontal (XZ) distance from the actor to point. */
f32 func_0024665C(LevelActor *arg0, CVector3 point)
{
    f32 dx = point.x - arg0->position.x;
    f32 dz = point.z - arg0->position.z;
    return dx * dx + dz * dz;
}
