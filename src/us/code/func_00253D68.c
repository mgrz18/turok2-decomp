/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022AE28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
typedef struct CVector3 { f32 x, y, z; } CVector3;
/* Turok 2: position 0x4 and region 0x10 (Rage Wars 0x8 / 0x14). */
typedef struct LevelActor {
    char pad0[4];
    CVector3 position;
    s32 region;
    char pad14[0x288 - 0x14];
} LevelActor;
/* Turok 2: LevelActor is 0x288 bytes, so the weapon actor follows at 0x288. */
typedef struct Shared_ActorView {
    LevelActor actor;
    LevelActor weapon;
} Shared_ActorView;
typedef struct Shared_World Shared_World;
extern Shared_World D_800F7078;
extern s32 func_00220A1C(Shared_World *, CVector3 *);
int func_00253D68(void *arg0, void *arg1) {
    int region;
    CVector3 *src = arg1;
    Shared_ActorView *view = arg0;
    region = ((int (*)(void *))func_00220A1C)(&D_800F7078);
    view->actor.position = *src;
    view->actor.region = region;
    view->weapon.position = *src;
    view->weapon.region = region;
    return region != 0;
}
