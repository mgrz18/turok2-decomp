/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FX/8026E5E0.CPP, func_8026E5E0_de, with Turok 2's layouts. */
#include "common.h"

/* Animation attachment events: for each attachment whose event bit is enabled
 * on the object, moves its offset through the bone matrix to world space and
 * plays the event: a footstep/impact sound (louder when the bone's surface is
 * water), the splash effects, or a dust puff of 5/10/15/20 units. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 x, y, z, w;
} Quatf;

typedef struct {
    f32 m[4][4];
} Mtx44;

typedef struct {
    /* 0x00 */ Vec3f offset;
    /* 0x0C */ u32 event;
    /* 0x10 */ u16 bone;
    /* 0x12 */ u8 pad12[0x14 - 0x12];
} Attachment;

typedef struct {
    s16 lifeBase;
    s16 lifeRange;
    s8 chance;
    s8 modifier;
    s8 countBase;
    s8 countRange;
} __attribute__((aligned(4))) PuffArgs;

typedef struct {
    /* 0x000 */ u8 pad0[0xA08];
    /* 0xA08 */ s16 vehicle;
} Owner;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 *def;
    /* 0x018 */ u8 pad18[0x174 - 0x18];
    /* 0x174 */ s8 state;
    /* 0x175 */ u8 pad175[0x180 - 0x175];
    /* 0x180 */ f32 depth;
    /* 0x184 */ u8 pad184[0x1A8 - 0x184];
    /* 0x1A8 */ Owner *owner;
    /* 0x1AC */ u8 pad1AC[0x214 - 0x1AC];
    /* 0x214 */ Vec3f splashPos;
    /* 0x220 */ Vec3f facing;
    /* 0x22C */ u8 pad22C[0x27C - 0x22C];
    /* 0x27C */ u32 events;
} Object;

extern u8 D_800F8928[];

extern u8 *func_002017D4(void *list, s32 index);
extern void func_0020F85C(Mtx44 *mtx, Vec3f *in, Vec3f *out);
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
extern void func_0020E810(Quatf *out, Vec3f *dir);
extern void func_00246690(Object *obj, s32 sound, Vec3f pos, s32 flags);
extern void func_002688C8(Object *obj, Vec3f pos, Vec3f *out, s32 *region, s32 a, s32 b);
extern void func_0022F350(void *sys, Object *p, Object *owner, s32 ownerId, s32 target, s32 type, Vec3f dir,
                          Quatf rot, Vec3f pos, s32 region, s32 arg10, s32 flags);
extern f32 func_0021F2B0(Object *obj);
extern void func_00236EA0(Object *obj, Object *owner, s32 type, s32 id, Vec3f pos, PuffArgs args);

void func_0026D710(Attachment *ents, s32 n, Mtx44 *mtxs, Object *obj, void *bones) {
    Vec3f pos;
    Vec3f mapped;
    Vec3f zero;
    Quatf rot;
    PuffArgs puff;
    s32 region;
    f32 len;
    s32 kind;
    s32 flag;
    s32 size;

    for (n = n - 1; n != -1; n--) {
        if (ents->event & obj->events) {
            flag = (func_002017D4(func_002017D4(bones, ents->bone), 1)[0x66] == 5) * 2;
            size = 0;
            func_0020F85C(&mtxs[ents->bone], &ents->offset, &pos);
            func_0020EF60(&pos, &pos, 1.0f / 65536.0f);
            switch (ents->event) {
            case 0x2:
                func_00246690(obj, 0x51, pos, flag);
                break;
            case 0x8:
                func_00246690(obj, 0x5C, pos, flag);
                break;
            case 0x1:
            case 0x4:
            case 0x10:
            case 0x20:
                break;
            case 0x40:
                obj->splashPos = pos;
                kind = *obj->def;
                if ((kind == 1 && obj->state == 0x21) || (kind == 0xB && obj->owner->vehicle == 0x23)) {
                    func_002688C8(obj, pos, &mapped, &region, 0, 1);
                    func_0020E810(&rot, &obj->facing);
                    zero.x = 0;
                    zero.y = 0;
                    zero.z = 0;
                    if (obj->depth < 15.0f) {
                        func_0022F350(D_800F8928, obj, obj, 0, 0, 0xE6, zero, rot, mapped, region, -1, flag | 1);
                    }
                    func_0022F350(D_800F8928, obj, obj, 0, 0, 0x8A, zero, rot, mapped, region, -1, flag | 1);
                }
                break;
            case 0x80:
                func_00246690(obj, 0xAC, pos, flag);
                break;
            case 0x100:
                func_00246690(obj, 0xAD, pos, flag);
                break;
            case 0x200:
                func_00246690(obj, 0xAE, pos, flag);
                break;
            case 0x400:
                func_00246690(obj, 0xAF, pos, flag);
                break;
            case 0x800:
                func_00246690(obj, 0xB0, pos, flag);
                break;
            case 0x1000:
                func_00246690(obj, 0xB1, pos, flag);
                break;
            case 0x2000:
                func_00246690(obj, 0xB2, pos, flag);
                break;
            case 0x4000:
                func_00246690(obj, 0xB3, pos, flag);
                break;
            case 0x8000:
                func_00246690(obj, 0xB4, pos, flag);
                break;
            case 0x10000:
                func_00246690(obj, 0xB5, pos, flag);
                break;
            case 0x20000:
                func_00246690(obj, 0xB6, pos, flag);
                break;
            case 0x40000:
                func_00246690(obj, 0xB7, pos, flag);
                break;
            case 0x80000:
                func_00246690(obj, 0xB8, pos, flag);
                break;
            case 0x100000:
                func_00246690(obj, 0xB9, pos, flag);
                break;
            case 0x200000:
                func_00246690(obj, 0xBA, pos, flag);
                break;
            case 0x400000:
                func_00246690(obj, 0xBB, pos, flag);
                break;
            case 0x20000000:
                func_00246690(obj, 0x1E9, pos, flag);
                break;
            case 0x40000000:
                func_00246690(obj, 0x1EA, pos, flag);
                break;
            case 0x80000000:
                func_00246690(obj, 0x1EB, pos, flag);
                break;
            case 0x8000000:
            case 0x10000000:
                func_00246690(obj, 0x11C, pos, flag);
                break;
            case 0x800000:
                size = 5;
                break;
            case 0x2000000:
                size = 0xF;
                break;
            case 0x1000000:
                size = 0xA;
                break;
            case 0x4000000:
                size = 0x14;
                break;
            }
            if (size != 0) {
                len = func_0021F2B0(obj);
                puff.lifeRange = size << 8;
                puff.chance = puff.modifier = puff.countBase = puff.countRange = 0;
                {
                    f32 t = 5.0f;
                    t = t / len;
                    puff.lifeBase = t * 256.0f;
                }
                func_00236EA0(obj, obj, 3, -1, pos, puff);
            }
        }
        ents++;
    }
}
