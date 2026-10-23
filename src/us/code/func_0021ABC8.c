/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_2.CPP, func_802476C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct CVector3 { f32 x, y, z; } CVector3;
typedef struct Entry802AB8DC {
    s32 offset;
    u8 pad4[4];
} Entry802AB8DC;
struct IntPair {
    int field_0;
    int field_4;
};
typedef struct CROMEventEntry {
    u16 frame;
    u16 effect;
    s16 bone;
    s16 pad6;
    CVector3 local;
    struct IntPair params;
} CROMEventEntry;
/* Turok 2 offsets (Rage Wars in parentheses): pos 0x4 (0x8), transform 0x58
 * (0x74), bone matrices 0x98 (0xB8), type id 0xB8 (0xE4), flags 0xD4 (0x100),
 * animation 0xD8 (0x104), bone mask 0x14C (0x17C). */
typedef struct ObjectLinks {
    char pad0[4];
    CVector3 pos;
    char pad10[0x48];
    char transform[0x40];
    char *boneMatrices;
    char pad9C[0x1C];
    u16 type;
    char padBA[0x1A];
    s32 flags;
    char anim[0x74];
    s32 boneMask;
} ObjectLinks;
/* Bone record: the mask bits sit at 0x6C. */
typedef struct BoneInfo {
    char pad0[0x6C];
    s32 mask;
} BoneInfo;
extern Entry802AB8DC D_800AD084[];
extern int D_800ACB94;
extern void func_0020F85C(void *arg0, void *arg1, void *arg2);
extern void *func_002017D4(void *, s32);
extern void func_002051F4(void *heap, s32 *resource);
extern CROMEventEntry *func_002199C8(void *);
extern s32 func_002199F4(void *);
extern s32 *func_0021F358(ObjectLinks *);
extern void func_0020D908(f32 *, char *);
extern void func_00236EA0(ObjectLinks *, ObjectLinks *, u16, s32, CVector3, struct IntPair);
/* Fires the animation events whose frame lies in [from, to). */
void func_0021ABC8(ObjectLinks *obj, void *anim, f32 from, f32 to) {
    CVector3 pos;
    CVector3 local;
    f32 matrix[16];
    CROMEventEntry *events;
    s32 count;
    s32 i;
    s32 checkMask;
    s32 fire;
    s32 *resource;
    void *table;
    if (!(obj->flags & 0x4000000) && anim != obj->anim) {
        return;
    }
    switch (obj->type) {
    case 0x44E:
    case 0x451:
    case 0x453:
        checkMask = 1;
        break;
    default:
        checkMask = 0;
        break;
    }
    table = 0;
    events = func_002199C8(anim);
    count = func_002199F4(anim);
    resource = 0;
    for (i = 0; i < count; i++) {
        if (from <= (f32)events[i].frame && (f32)events[i].frame < to) {
            fire = 1;
            if (events[i].bone == -1) {
                local.x = events[i].local.x;
                local.y = events[i].local.z;
                local.z = events[i].local.y;
                func_0020F85C(obj->transform, &local, &pos);
            } else {
                if (checkMask && (D_800AD084[events[i].effect].offset & 4)) {
                    if (resource == 0) {
                        resource = func_0021F358(obj);
                        if (resource == 0) {
                            goto masked;
                        }
                        table = func_002017D4((void *)*resource, 3);
                    }
                    fire = ((BoneInfo *)func_002017D4(func_002017D4(table, events[i].bone), 1))->mask & obj->boneMask;
                }
            masked:
                if (fire == 0) {
                    continue;
                }
                if (obj->boneMatrices != 0) {
                    func_0020D908(matrix, obj->boneMatrices + (events[i].bone << 6));
                    func_0020F85C(matrix, &events[i].local, &pos);
                } else {
                    pos = obj->pos;
                }
            }
            if (fire != 0) {
                func_00236EA0(obj, obj, events[i].effect, -1, pos, events[i].params);
                D_800ACB94++;
            }
        }
    }
    if (resource != 0) {
        func_002051F4(0, resource);
    }
}
