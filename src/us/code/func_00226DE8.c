/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028E768.c, func_0028E768.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct Entity { /* Turok 2 layout */
    u8 pad0[0x4];
    Vec3f pos;          /* 0x04 */
    u8 pad10[0x4];
    CIntelligence *def;      /* 0x14 */
    u8 pad18[0xA0];
    u16 unkB8;          /* 0xB8: actor type id (Rage Wars: 0xE4) */
    u8 padBA[0x8A];
    s32 unk144;         /* Rage Wars: 0x174 */
    u8 pad148[0x2C];
    s8 unk174;          /* Rage Wars: 0x1A4 */
} Entity;
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
s32 func_00226DE8(Entity *e, Vec3f *target) {
    Vec3f d;
    f32 dist;
    u32 t;
    func_0020EF2C(&d, &e->pos, target);
    dist = d.x * d.x + d.y * d.y + d.z * d.z;
    if (e->def->m_Type == 1) {
        t = e->unkB8;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0;
        }
        if (e->unk144 != 0) {
            if (e->unk174 != 0x3D) {
                if (dist <= 6553600.0f) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
