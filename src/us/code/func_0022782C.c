/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028FD2C_12.CPP, func_802905F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;

typedef struct CVector3 {
    f32 x, y, z;
} CVector3;

/* Turok 2 layout (Rage Wars offsets in parentheses). */
typedef struct SceneEffect {
    char pad0[0x14C];
    f32 minX;                  /* 0x14C (0x17C) */
    f32 minY;                  /* 0x150 */
    f32 minZ;                  /* 0x154 */
    f32 maxX;                  /* 0x158 */
    f32 maxY;                  /* 0x15C */
    f32 maxZ;                  /* 0x160 */
    char pad164[0x198 - 0x164];
    f32 lifetime;              /* 0x198 (0x1C8) */
    char pad19C[0x1AC - 0x19C];
    struct SceneEffect *next;  /* 0x1AC (0x1DC) */
} SceneEffect;

typedef struct GameplayCamera {
    char pad0[0x2D8];
    char clipPlanes[0x60];     /* 0x2D8 (0x2F0) */
    CVector3 clipBounds[2];    /* 0x338 (0x350) */
} GameplayCamera;

typedef struct WorldRecAt3C08 {
    char pad0[0x3604];
    SceneEffect *head;         /* 0x3604 (0x3C04) */
} WorldRecAt3C08;

extern float D_800A6064;
extern f32 D_800A6068;
extern s32 D_800AB8B0;
extern f32 D_800AB8C0;
extern void func_0020B5D4();
extern void func_0020B624();
extern s32 func_0026F60C(void *arg0, void *arg1);
extern void func_002152AC(void *arg0);

/* Draw the scene effects whose bounds are inside the camera's clip volume. */
void func_0022782C(char *arg0, char *arg1) {
    WorldRecAt3C08 *world = (void *)arg0;
    GameplayCamera *camera = (void *)arg1;
    SceneEffect *entry;
    f32 value;
    f32 upper;
    f32 scale;
    f32 zero;
    func_0020B5D4();
    entry = world->head;
    if (entry != 0) {
        zero = 0.0f;
        upper = D_800A6064;
        scale = D_800A6068;
        do {
            if ((camera->clipBounds[1].x > entry->minX) &&
                (camera->clipBounds[0].x < entry->maxX) &&
                (camera->clipBounds[1].z > entry->minZ) &&
                (camera->clipBounds[0].z < entry->maxZ) &&
                (camera->clipBounds[1].y > entry->minY) &&
                (camera->clipBounds[0].y < entry->maxY) &&
                (func_0026F60C(camera->clipPlanes, &entry->minX) != 0)) {
                value = entry->lifetime;
                D_800AB8B0 = 0;
                if ((value > zero) && (value <= upper)) {
                    D_800AB8B0 = 1;
                    D_800AB8C0 = value * scale;
                }
                func_002152AC(entry);
                D_800AB8B0 = 0;
            }
            entry = entry->next;
        } while (entry != 0);
    }
    func_0020B624();
}
