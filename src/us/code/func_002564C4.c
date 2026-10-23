/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894_4.CPP, func_8022DB04_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct Object126;
typedef struct Object126 Object126;
struct Actor126;
typedef struct Actor126 Actor126;
extern float D_800A71D4;
extern float D_800A71D0;
struct Actor126;
struct Actor126 {         /* Turok 2 offsets (Rage Wars in comments) */
    u8 pad0[0xD4];
    s32 flags;            /* 0x0D4 (0x100) */
    u8 padD8[0xCC];
    f32 value1A4;         /* 0x1A4 (0x1D4) */
    u8 pad1A8[0x862];
    s16 stateA0A;         /* 0xA0A (0x652) */
    u8 padA0C[0xC0];
    f32 offsetACC;        /* 0xACC (0x728) */
};
extern float D_800A71CC;
struct Object126;
struct Object126 {
    u8 pad0[0x18];
    f32 x;                /* 0x18 (0x1C) */
    u8 pad1C[4];
    f32 y;                /* 0x20 (0x24) */
    u8 pad24[0x2C];
    f32 angle;            /* 0x50 (0x6C): yaw */
};
extern void func_002564C4(Actor126 *actor, Object126 *arg1);
void func_002564C4(Actor126 *actor, Object126 *arg1) {
    Object126 *object;
    f32 angle;
    f32 object_angle;
    f32 sine;
    f32 cosine;
    actor->flags |= 0x800000;
    actor->value1A4 = D_800A71CC;
    if (actor->stateA0A == 14) {
        object = arg1;
        object_angle = object->angle;
        angle = D_800A71D0;
        sine = func_002982F0(object_angle + angle);
        cosine = func_002974C0(object->angle + angle);
        sine *= D_800A71D4;
        cosine *= D_800A71D4;
        object->x += sine;
        object->y += cosine;
        object->angle += actor->offsetACC;
        actor->offsetACC = 0.0f;
    }
}
