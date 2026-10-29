/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00276D20.c, func_00276D20, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* A placed volume as the level data stores it: fixed point position, rotation, scale and size. */
typedef struct VolumeRec {
    /* 0x00 */ s32 pos[3];
    /* 0x0C */ s32 rot[3];
    /* 0x18 */ s32 scale[3];
    /* 0x24 */ s32 width;
    /* 0x28 */ s32 height;
    /* 0x2C */ u8 pad2C[3];
    /* 0x2F */ u8 mode;
    /* 0x30 */ u8 pad30[7];
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 pad38[3];
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ u32 unk3C[3];
} VolumeRec;

/* The volume built from it. */
typedef struct Volume {
    /* 0x00 */ f32 width;
    /* 0x04 */ f32 height;
    /* 0x08 */ f32 unk8[3];
    /* 0x14 */ Vec3f dir;
    /* 0x20 */ u8 unk20[0x40];
    /* 0x60 */ f32 mtx[16];
    /* 0xA0 */ f32 bounds[6];
    /* 0xB8 */ u8 unkB8;
    /* 0xB9 */ u8 aligned;
    /* 0xBA */ u8 unkBA;
    /* 0xBB */ u8 mode;
} Volume;

extern s32 D_8011AAD8;

void func_0020C208(void *out, f32 *mtx);
void func_0020F79C(f32 *mtx);
void func_0020F9D4(f32 *mtx, Vec3f *in, Vec3f *out);
void func_00210318(f32 *mtx, f32 x, f32 y, f32 z);
void func_00210478(f32 *mtx, f32 x, f32 y, f32 z);
void func_00210790(f32 *mtx, f32 angle);
void func_00210968(f32 *mtx, f32 angle);
void func_00210B38(f32 *mtx, f32 angle);
void func_0026B9E0(f32 *bounds, s32 count, Vec3f *points);

/* Builds a volume from its level record: its matrix (scale, rotations about z, y and x, then the
 * position), its size, whether it is unrotated (each angle within 0.01) and the bounds of its
 * eight transformed corners. In mode 1 the rotation only aims dir, the volume itself stays upright. */
void func_00211F64(Volume *vol, VolumeRec *rec) {
    Vec3f pos;
    Vec3f rot;
    Vec3f scale;
    Vec3f corners[8];
    Vec3f down;
    Vec3f aim;
    Vec3f corner;
    s32 i;
    s32 aligned;
    Vec3f *out;
    f32 *mtx;
    f32 x;
    f32 y;
    f32 z;

    vol->mode = rec->mode;
    pos.x = rec->pos[0] * 2.384185791015625e-05f;
    pos.y = rec->pos[1] * 2.384185791015625e-05f;
    pos.z = rec->pos[2] * 2.384185791015625e-05f;
    if (vol->mode != 1) {
        rot.x = rec->rot[0] * 9.313225746154785e-09f;
        rot.y = rec->rot[1] * 9.313225746154785e-09f;
        rot.z = rec->rot[2] * 9.313225746154785e-09f;
    } else {
        down.x = 0.0f;
        down.y = -1.0f;
        down.z = 0.0f;
        aim.x = rec->rot[0] * 9.313225746154785e-09f;
        aim.y = rec->rot[1] * 9.313225746154785e-09f;
        aim.z = rec->rot[2] * 9.313225746154785e-09f;
        mtx = vol->mtx;
        func_0020F79C(mtx);
        func_00210B38(mtx, aim.x);
        func_00210968(mtx, aim.y);
        func_0020F9D4(mtx, &down, &vol->dir);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
    }
    aligned = 1;
    if (rot.x < 0.0f) {
        if (0.01f < -rot.x) {
            goto not_x;
        }
    } else if (0.01f < rot.x) {
    not_x:
        aligned = 0;
    }
    if (rot.y < 0.0f) {
        if (0.01f < -rot.y) {
            goto not_y;
        }
    } else if (0.01f < rot.y) {
    not_y:
        aligned = 0;
    }
    if (rot.z < 0.0f) {
        if (0.01f < -rot.z) {
            goto not_z;
        }
    } else if (0.01f < rot.z) {
    not_z:
        aligned = 0;
    }
    vol->aligned = 0;
    if (aligned != 0) {
        vol->aligned = 1;
    }
    scale.x = rec->scale[0] * 2.384185791015625e-05f;
    scale.y = rec->scale[1] * 2.384185791015625e-05f;
    scale.z = rec->scale[2] * 2.384185791015625e-05f;
    vol->width = 2.0f * rec->width * 4.656612873077393e-10f;
    do {
        vol->height = 2.0f * rec->height * 4.656612873077393e-10f;
    } while (0);
    if (D_8011AAD8 & 0x4000) {
        vol->unk8[0] = 0.0f;
        vol->unk8[1] = 0.0f;
        vol->unk8[2] = 0.0f;
    } else {
        vol->unk8[0] = (f32)rec->unk3C[0] * 2.3748725652694702e-06f;
        vol->unk8[1] = (f32)rec->unk3C[1] * 2.3748725652694702e-06f;
        vol->unk8[2] = (f32)rec->unk3C[2] * 2.3748725652694702e-06f;
    }
    mtx = vol->mtx;
    {
        /* the ROM sets up the call's argument before these stores */
        register f32 *arg asm("$4") = mtx;

        vol->unkB8 = rec->unk37;
        i = 0;
        vol->unkBA = rec->unk3B;
        func_0020F79C(arg);
    }
    func_00210478(mtx, scale.x, scale.y, scale.z);
    func_00210B38(mtx, rot.z);
    func_00210968(mtx, rot.y);
    func_00210790(mtx, rot.x);
    func_00210318(mtx, pos.x, pos.y, pos.z);
    func_0020C208(vol->unk20, mtx);
    out = corners;
    do {
        x = 1.0f;
        y = x;
        z = x;
        if (i & 1) {
            x = -1.0f;
        }
        if (i & 2) {
            y = -1.0f;
        }
        if (i & 4) {
            z = -1.0f;
        }
        corner.x = x;
        corner.y = y;
        corner.z = z;
        func_0020F9D4(vol->mtx, &corner, out);
        out++;
        i++;
    } while (i < 8);
    func_0026B9E0(vol->bounds, 8, corners);
}
