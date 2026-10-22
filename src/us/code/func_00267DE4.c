/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DDDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Model;
struct Model {
    s32 kind;
    char pad4[0x10];
    u32 flags;
    char pad18[0x34];
    s32 slots[8];
};
struct Model;
struct Model;
struct Model;
struct Model;
typedef struct Model Model;
struct Model;
struct ModelBlend;
struct ModelBlend {
    s32 kind;
    char pad4[0x28];
    f32 blend;
    u8 pad30[4];
    CVector3 effectOffset;
    u8 pad40[4];
    s32 collisionFlags;
    char pad48[4];
    u32 collisionOptions;
    char pad50[2];
    u16 materialFlags;
    char pad54[4];
    u8 byte58;
    u8 byte59;
    char pad5A[0x9A];
    f32 height;
};
struct ModelBlend;
typedef struct ModelBlend ModelBlend;
struct Model;
struct Model;
struct Model;
struct Model;
struct Model;
struct Model;
struct Model;
struct ModelBlend;
struct Model;
struct ModelBlend;
struct Object;
struct Model;
struct Model;
struct Model;
struct ModelBlend;
struct Object {
    char pad[0x14];
    struct ModelBlend *owner; /* 0x14 in Turok 2 */
    u8 reserved18[0x740 - 0x18];
    u8 effect740;
};
s32 func_00267DE4(void *arg0)
{
    struct Object *object = arg0;
    struct Model *model;
    s32 kind;
    model = (struct Model *)object->owner;
    kind = model->kind;
    if ((kind == 1) || (kind == 4)) {
        if (model) {
            return model->flags & 0x80;
        } else {
            return model->flags & 0x80;
        }
    }
    return 0;
}
