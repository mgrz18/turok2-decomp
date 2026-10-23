/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_9.CPP, func_8024B53C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
struct LevelActor;
struct CVector3_t;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct Box;
typedef struct Box Box;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Box;
struct Box {
    CVector3 min;
    CVector3 max;
};
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
typedef struct ActorDefinitionHeader {
    s32 kind;
    s32 gate;
    u8 pad8[4];
    s16 character;
    s8 first;
    u8 padF;
    s8 second;
    u8 pad11;
    s8 send;
    u8 pad13;
    s32 flags;
} ActorDefinitionHeader;
typedef struct LevelActor LevelActor;
struct LevelActor {               /* Turok 2 layout (Rage Wars offsets in comments) */
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    CVector3 position0;           /* 0x04 (0x08) */
    s32 model;                    /* 0x10 (0x14) */
    ActorDefinitionHeader *definition; /* 0x14 (0x18) */
    CVector3 position1;           /* 0x18 (0x1C) */
    u8 pad24[0x10];
    CVector3 scale;               /* 0x34 (0x50) */
    CQuatern rotation;            /* 0x40 (0x5C) */
    f32 yaw;                      /* 0x50 (0x6C) */
    f32 eyeHeight;                /* 0x54 (0x70) */
    f32 transform[16];            /* 0x58 (0x74) */
};
struct LevelActor;
struct LevelActor;
struct ActorDefinitionHeader;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
extern s32 Cinema_PathPlaying(void);
extern void func_00268F3C(CQuatern *arg0, void *arg1);
extern void func_00210F68(CQuatern *arg0, CQuatern *arg1, CQuatern *arg2);
extern void func_00211114(void *, void *);
extern void func_0021034C(void *arg0, f32 sx, f32 sy, f32 sz);
extern s32 func_0020F510(f32 *arg0, f32 arg1);
extern void func_00210318(char *, f32, f32, f32);
extern void func_00210C3C(void *);
extern f32 D_800DD78C;
void func_0021E840(void *arg0) {
    CQuatern trig;
    CQuatern product;
    CQuatern copy;
    CQuatern *copy_ptr;
    CQuatern *product_ptr;
    char *transform;
    f32 scale;
    f32 sine;
    f32 angle;
    copy = ((struct LevelActor *)(arg0))->rotation;
    copy_ptr = &copy;
    if (Cinema_PathPlaying() != 0) {
        func_00268F3C(copy_ptr, arg0);
    }
    angle = ((struct LevelActor *)(arg0))->yaw;
    scale = (0.5f);
    sine = func_002982F0(angle * scale);
    trig.x = 0.0f;
    trig.y = sine;
    trig.z = 0.0f;
    angle = ((struct LevelActor *)(arg0))->yaw * scale;
    D_800DD78C = sine;
    trig.w = func_002974C0(angle);
    product_ptr = &product;
    func_00210F68(product_ptr, &trig, copy_ptr);
    transform = &((struct LevelActor *)(arg0))->transform[0];
    func_00211114(product_ptr, transform);
    func_0021034C(transform, ((struct LevelActor *)(arg0))->scale.x,
                    ((struct LevelActor *)(arg0))->scale.y,
                    ((struct LevelActor *)(arg0))->scale.z);
    func_0020F510(&((struct LevelActor *)(arg0))->position0.x, 20000.0f);
    func_00210318(transform, ((struct LevelActor *)(arg0))->position0.x,
                   ((struct LevelActor *)(arg0))->position0.y,
                   ((struct LevelActor *)(arg0))->position0.z);
    func_00210C3C(transform);
}
