/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028E83C.c, func_0028E83C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct ActorDef {
    s32 type;
} ActorDef;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
/* Turok 2 layout (Rage Wars: pos 0x8, def 0x18, typeId 0xE4). */
typedef struct Entity {
    u8 kind;                /* 0x00 */
    u8 pad1[3];
    Vec3f pos;              /* 0x04 */
    s32 unk10;
    ActorDef *def;          /* 0x14 */
    u8 pad18[0xB8 - 0x18];
    u16 typeId;             /* 0xB8 */
} Entity;
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
s32 func_00226EBC(Entity *e, Vec3f *target) {
    Vec3f d;
    f32 dist;
    u32 t;
    if (*(u8 *)e == 1 && e->def->type == 7) {
        func_0020EF2C(&d, &e->pos, target);
        dist = d.x * d.x + d.z * d.z;
        d.y = 0;
        if (262144.0f < dist) {
            return 0;
        }
        return 0xFF;
    }
    if (e->def->type == 1) {
        t = e->typeId;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0xFF;
        }
    }
    return 0;
}
