/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00276D20.c, func_00276D20, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"

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
    /* 0x14 */ CVector3 dir;
    /* 0x20 */ u8 unk20[0x40];
    /* 0x60 */ f32 mtx[16];
    /* 0xA0 */ f32 bounds[6];
    /* 0xB8 */ u8 unkB8;
    /* 0xB9 */ u8 aligned;
    /* 0xBA */ u8 unkBA;
    /* 0xBB */ u8 mode;
} Volume;

extern s32 gEngineApp_m_Settings;

void CMtxF__InvertAffine(void *out, f32 *mtx);
void CMtxF__Identity(f32 *mtx);
void CMtxF__TransformPointProjective(f32 *mtx, CVector3 *in, CVector3 *out);
void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__ScaleColumns(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__RotateX(f32 *mtx, f32 angle);
void CMtxF__RotateY(f32 *mtx, f32 angle);
void CMtxF__RotateZ(f32 *mtx, f32 angle);
void BoundingBox_FromPoints(f32 *bounds, s32 count, CVector3 *points);

/* Builds a volume from its level record: its matrix (scale, rotations about z, y and x, then the
 * position), its size, whether it is unrotated (each angle within 0.01) and the bounds of its
 * eight transformed corners. In mode 1 the rotation only aims dir, the volume itself stays upright. */
void LightVolume_Construct(Volume *vol, VolumeRec *rec) {
    CVector3 pos;
    CVector3 rot;
    CVector3 scale;
    CVector3 corners[8];
    CVector3 down;
    CVector3 aim;
    CVector3 corner;
    s32 i;
    s32 aligned;
    CVector3 *out;
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
        CMtxF__Identity(mtx);
        CMtxF__RotateZ(mtx, aim.x);
        CMtxF__RotateY(mtx, aim.y);
        CMtxF__TransformPointProjective(mtx, &down, &vol->dir);
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
    if (gEngineApp_m_Settings & 0x4000) {
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
        CMtxF__Identity(arg);
    }
    CMtxF__ScaleColumns(mtx, scale.x, scale.y, scale.z);
    CMtxF__RotateZ(mtx, rot.z);
    CMtxF__RotateY(mtx, rot.y);
    CMtxF__RotateX(mtx, rot.x);
    CMtxF__Translate(mtx, pos.x, pos.y, pos.z);
    CMtxF__InvertAffine(vol->unk20, mtx);
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
        CMtxF__TransformPointProjective(vol->mtx, &corner, out);
        out++;
        i++;
    } while (i < 8);
    BoundingBox_FromPoints(vol->bounds, 8, corners);
}
