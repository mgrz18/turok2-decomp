/* One source file from func_0027D160 to func_0027E000: the camera advance
 * and its display list setup. func_0027E000's .rodata starts at 0x800A99E4,
 * 4 past an 8-byte boundary, right after func_0027D160's pool (its last
 * word, a 5.0f, at 0x800A99E0); an object of its own would 8-align it.
 *
 * func_0027D160: after AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_6.CPP, func_80234FEC_de, with Turok 2's layouts.
 * func_0027E000: after the same file's Camera_SetupViewportAndFog. */
#include "common.h"
#include "structs/player.h"
#include "gfx.h"
#include "structs/camera.h"
#include "structs/game.h"

/* A camera mode: its update, then a word. */
typedef struct {
    /* 0x0 */ void (*m_Update)(CCamera *cam);
    /* 0x4 */ s32 _pad;
} CCameraMode;

/* The game global at D_80119870: what this reads of it. */
typedef struct {
    /* 0x0000 */ u8 _pad0[0x1281];
    /* 0x1281 */ u8 m_bWideViewVolume;
    /* 0x1282 */ u8 _pad1282[0x196];
    /* 0x1418 */ s32 m_Mode;               /* D_8011AC88: game mode */
} Game80119870;

typedef struct {
    /* 0x00 */ u8 _pad0[0x1C];
    /* 0x1C */ u32 m_VisBits;
} CFloor;

extern CPlayer *D_80119890;
extern f32 D_800B6D28;          /* the frame's time step */
extern CCameraMode D_800B6CB4[];
extern s32 D_800B6D1C;          /* which of the two frame buffers is drawn */
extern f32 D_800DD78C;          /* the last sine taken */
extern CCamera D_801198F0;      /* the main camera */
/* The camera pool, seen from its main camera. */
#define D_801198B0 (*(CCameraPool *) ((u8 *) &D_801198F0 - 0x40))
extern Game80119870 D_80119870;
extern s32 D_801198E8;          /* the pool wants on-screen projections */
extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern s32 D_800F7078[];
extern s32 D_800B6D00;
void func_0027CAB0(CCamera *cam);
s32 Cinema_PathPlaying(void);
void func_00297860(CMtxF m, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);
void func_00284254(CCamera *cam);
void func_00281D1C(CCamera *cam, CVector3 *offset);
f32 func_002982F0(f32 x);       /* sinf */
f32 func_002974C0(f32 x);       /* cosf */
void func_00210F68(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void func_00211114(CQuaternion *q, CMtxF m);
void func_0020FC24(CMtxF m, f32 x, f32 y, f32 z);
void func_0027BE90(CShake *shake, CVector3 pos, CVector3 *out);
u32 func_0027BD30(CShakeAxis *axis);
void func_0020C618(CMtxF out, CMtxF a, CMtxF b);
void func_0020F85C(CMtxF m, CVector3 *in, CVector3 *out);
void func_002101A0(CMtxF m, CVector3 *pos);
void func_0020C208(CMtxF out, CMtxF m);
void func_002105A4(CMtxF m, f32 angle);
void func_0020FF80(CMtxF out, CMtxF m);
void func_002117DC(f32 *value, f32 target, f32 rate);
f32 func_00284390(CCamera *cam);
f32 func_00298470(f32 x);       /* sqrtf */
f32 func_0021153C(f32 x);
f32 func_002843D4(void);
void func_0029815C(CMtxF m, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void func_0020FC74(CMtxF m, f32 x, f32 y, f32 z);
void func_0020C890(CMtxF out, CMtxF a, CMtxF b);
void func_0020F7EC(CMtxF m);
void func_0020CC94(CMtxF m, Mtx *out);
void func_00210790(CMtxF m, f32 angle);
void func_00210968(CMtxF m, f32 angle);
void func_0020F908(CMtxF m, CVector3 *in, CVector3 *out, u32 count);
void func_0026F080(CPlane *planes, CVector3 *eye, CVector3 *c0, CVector3 *c1, CVector3 *c2, CVector3 *c3);
void func_0026B9E0(CVector3 *box, s32 count, CVector3 *points);
void func_0020F79C(CMtxF m);
void func_00282E4C(void *sky, CCamera *cam);
void func_004169C8(void *menus);
CFloor *func_00220A1C(s32 *world, CVector3 *pos);
f32 func_0026BE60(CFloor *floor, f32 x, f32 z);

#define MIN1(x) (((x) > 1.0f) ? 1.0f : (x))

/* Advances a camera (LibTEngine's CCamera Advance): see docs/CAMERA.md. */
void func_0027D160(CCamera *cam) {
    CMtxF mLookAt, mRot, mYOffset, mGround, mTrans, mPi, mInv, mView, m, mOrient;
    CMtxF mProj, mFlip, mLight, mOnScreen;
    CQuaternion qX, qY, qZ, qXY, q;
    CVector3 corners[5];
    CVector3 tremor, shake, shakeOffset;
    CQuaternion qGround;
    CPlayer *node;
    CPlayer *player;
    LookAt *lookAt;
    void (*update)(CCamera *);
    CShake *s;
    CFloor *floor;
    f32 rotX, rotZ, rotY, rotYOffset, posX, posY, posZ, yOffset, zoomBlend, zoomFov;
    f32 half, a, sn, swim, inv;
    f32 fov, aspect, t, ty, far, halfX, halfY, radius, r15, v;
    f32 x, y, z, height;
    /* Block-local temporaries: separate variables keep each in its own block (GCC's local allocator). */
    f32 cornerX, cornerY, yawCam, yawPath, ang, above, depth;
    f32 boxX, boxY, boxZ;
    s32 n;

    for (node = D_80119890; node != NULL; node = node->m_pNext) {
        if (node->m_pCamera == cam) {
            player = node;
            goto found;
        }
    }
    player = NULL;
found:
    cam->m_Time += D_800B6D28;
    func_0027CAB0(cam);
    if (Cinema_PathPlaying() == 0) {
        update = D_800B6CB4[cam->m_Mode].m_Update;
        if (update != NULL) {
            update(cam);
        }
    }
    lookAt = &cam->m_LookAt[D_800B6D1C];
    func_00297860(mLookAt, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    func_00284254(cam);
    rotX = cam->m_ViewParams.m_vRotOffset.x;
    rotZ = cam->m_ViewParams.m_vRotOffset.z;
    posX = cam->m_ViewParams.m_vPos.x;
    posY = cam->m_ViewParams.m_vPos.y;
    posZ = cam->m_ViewParams.m_vPos.z;
    rotY = cam->m_ViewParams.m_RotY;
    rotYOffset = cam->m_ViewParams.m_vRotOffset.y;
    yOffset = cam->m_ViewParams.m_YPosOffset;
    qGround = cam->m_ViewParams.m_qGround;
    zoomBlend = cam->m_ViewParams.m_ZoomBlend;
    zoomFov = cam->m_ViewParams.m_ZoomFov;
    if (Cinema_PathPlaying() == 0) {
        func_00281D1C(cam, &tremor);
        rotZ += tremor.z;
        rotX += tremor.x;
        posY += tremor.y;
        half = 0.5f;
        a = rotZ * half;
        sn = func_002982F0(a);
        D_800DD78C = sn;
        qZ.x = 0.0f;
        qZ.y = 0.0f;
        qZ.z = sn;
        qZ.w = func_002974C0(a);
        a = rotX * half;
        sn = func_002982F0(a);
        D_800DD78C = sn;
        qX.x = sn;
        qX.y = 0.0f;
        qX.z = 0.0f;
        qX.w = func_002974C0(a);
        yawCam = (rotY + rotYOffset) * half;
        sn = func_002982F0(yawCam);
        D_800DD78C = sn;
        qY.x = 0.0f;
        qY.y = sn;
        qY.z = 0.0f;
        qY.w = func_002974C0(yawCam);
        func_00210F68(&qXY, &qX, &qY);
        func_00210F68(&cam->m_qViewAlignNoZ, &qXY, &qGround);
        func_00210F68(&cam->m_qViewAlign, &qZ, &cam->m_qViewAlignNoZ);
        func_00210F68(&q, &qZ, &qXY);
        func_00211114(&q, mRot);
        func_0020FC24(mYOffset, 0.0f, yOffset, 0.0f);
        func_00211114(&qGround, mGround);
        shake.x = 0.0f;
        shake.y = 0.0f;
        shake.z = 0.0f;
        func_0027BE90(&cam->m_Shake, cam->m_Shake.m_vPos, &shake);
        func_0027BD30(&cam->m_Shake.m_XAxis);
        func_0027BD30(&cam->m_Shake.m_YAxis);
        func_0027BD30(&cam->m_Shake.m_ZAxis);
        if (cam != &D_801198B0.m_MainCamera) {
            for (s = (CShake *) D_801198B0.m_ShakeActiveList.m_pHead; s != NULL; s = s->m_pNext) {
                func_0027BE90(s, cam->m_ViewParams.m_vPos, &shake);
            }
        }
        func_0020C618(m, mRot, mGround);
        func_0020F85C(m, &shake, &shakeOffset);
        posX += shakeOffset.x;
        posY += shakeOffset.y;
        posZ += shakeOffset.z;
        func_0020C618(m, mRot, mYOffset);
        func_0020C618(mOrient, m, mGround);
        func_0020FC24(mTrans, posX, posY, posZ);
        func_0020C618(cam->m_mfViewOrient, mOrient, mTrans);
    } else {
        half = 0.5f;
        a = rotZ * half;
        sn = func_002982F0(a);
        D_800DD78C = sn;
        qZ.x = 0.0f;
        qZ.y = 0.0f;
        qZ.z = sn;
        qZ.w = func_002974C0(a);
        a = rotX * half;
        sn = func_002982F0(a);
        D_800DD78C = sn;
        qX.x = sn;
        qX.y = 0.0f;
        qX.z = 0.0f;
        qX.w = func_002974C0(a);
        yawPath = (rotY + rotYOffset) * half;
        sn = func_002982F0(yawPath);
        D_800DD78C = sn;
        qY.x = 0.0f;
        qY.y = sn;
        qY.z = 0.0f;
        qY.w = func_002974C0(yawPath);
        func_00210F68(&qXY, &qX, &qY);
        cam->m_qViewAlignNoZ = qXY;
        func_00210F68(&cam->m_qViewAlign, &qZ, &cam->m_qViewAlignNoZ);
        func_00211114(&qXY, mRot);
        func_0020FC24(mTrans, posX, posY, posZ);
        func_0020C618(cam->m_mfViewOrient, mRot, mTrans);
    }
    func_002101A0(cam->m_mfViewOrient, &cam->m_vPos);
    cam->m_vRotation.x = rotX;
    cam->m_vRotation.y = rotY;
    cam->m_vRotation.z = rotZ;
    func_0020C208(cam->m_mfInverseViewOrient, cam->m_mfViewOrient);
    func_002105A4(mPi, 3.1415927f);
    func_0020C618(m, mPi, cam->m_mfViewOrient);
    func_0020C208(mInv, m);
    func_0020C618(mView, mInv, mLookAt);
    func_0020FF80(cam->m_mfView, mView);
    if (player != NULL && (player->m_ModeFlags & 0x40) && cam->m_ViewParams.m_nType == 0 && Cinema_PathPlaying() == 0) {
        cam->m_XScale = MIN1(cam->m_WaterWobbleXScale * 0.5f * (func_002982F0(cam->m_WaterWobbleXAngle) + 1.0f) + 0.8f);
        cam->m_YScale = MIN1(cam->m_WaterWobbleYScale * 0.5f * (func_002982F0(cam->m_WaterWobbleYAngle) + 1.0f) + 0.9f);
        cam->m_WaterWobbleXAngle += D_800B6D28 * cam->m_WaterWobbleXSpeed;
        cam->m_WaterWobbleYAngle += D_800B6D28 * cam->m_WaterWobbleYSpeed;
        swim = player->m_TotalSpeed * 2.5f;
        if (swim < 0.0f) {
            swim = 0.0f;
        } else if (1.0f < swim) {
            swim = 1.0f;
        }
        func_002117DC(&cam->m_WaterWobbleXScale, 0.1f, 0.5f);
        func_002117DC(&cam->m_WaterWobbleYScale, 0.05f, 0.5f);
        inv = 1.0f - swim;
        func_002117DC(&cam->m_WaterWobbleXSpeed, swim * 0.104719765f + inv * 0.06981318f, 0.16666667f);
        func_002117DC(&cam->m_WaterWobbleYSpeed, swim * 0.087266475f + inv * 0.03490659f, 0.16666667f);
    } else {
        cam->m_YScale = 1.0f;
        cam->m_XScale = 1.0f;
    }
    if (Cinema_PathPlaying() != 0) {
        fov = func_00284390(cam);
    } else {
        fov = cam->m_FieldOfView + zoomBlend * (zoomFov - cam->m_FieldOfView);
    }
    fov = fov * 0.017453294f;
    aspect = cam->m_Viewport.m_Width / cam->m_Viewport.m_Height;
    ty = func_002982F0(fov) / func_002974C0(fov);
    t = ty * 1.2792794f;
    ty = t / aspect;
    ty = ty / cam->m_YScale;
    t = t / cam->m_XScale;
    fov = func_0021153C(func_00298470(1.0f / (ty * ty + 1.0f)));
    fov = fov * 57.295776f;
    far = cam->m_FarClip;
    aspect = t / ty;
    cam->m_AspectRatio = aspect;
    cam->m_Fov = fov;
    halfX = far * t;
    halfY = far * ty;
    if (Cinema_PathPlaying() != 0) {
        n = func_002843D4();
        if (n > 0) {
            cam->m_FarClip = n;
        }
    }
    func_0029815C(mProj, &cam->m_PerspNorm, fov, aspect, 16.0f, cam->m_FarClip, 1.0f - zoomBlend * 0.95f);
    func_0020FC74(mFlip, -1.0f, 1.0f, 1.0f);
    func_0020C890(m, mView, mProj);
    func_0020C890(cam->m_mfProjection, m, mFlip);
    func_0020F7EC(cam->m_mfProjection);
    func_0020CC94(cam->m_mfProjection, &cam->m_mProjection[D_800B6D1C]);
    func_0020FF80(mLight, mView);
    if (D_80119870.m_Mode == 4) {
        func_00210790(mLight, cam->m_Time * 0.010908309f);
        func_00210968(mLight, cam->m_Time * 0.008726647f);
    }
    func_0020F7EC(mLight);
    v = mLight[0][0] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x08] = (s32) v;
    v = mLight[1][0] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x09] = (s32) v;
    v = mLight[2][0] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x0A] = (s32) v;
    v = mLight[0][1] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x18] = (s32) v;
    v = mLight[1][1] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x19] = (s32) v;
    v = mLight[2][1] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x1A] = (s32) v;
    lookAt->l[0x00] = 0;
    lookAt->l[0x01] = 0;
    lookAt->l[0x02] = 0;
    lookAt->l[0x03] = 0;
    lookAt->l[0x04] = 0;
    lookAt->l[0x05] = 0;
    lookAt->l[0x06] = 0;
    lookAt->l[0x07] = 0;
    lookAt->l[0x10] = 0;
    lookAt->l[0x11] = 0x80;
    lookAt->l[0x12] = 0;
    lookAt->l[0x13] = 0;
    lookAt->l[0x14] = 0;
    lookAt->l[0x15] = 0x80;
    lookAt->l[0x16] = 0;
    lookAt->l[0x17] = 0;
    if (!D_80119870.m_bWideViewVolume) {
        ang = fov * aspect * 0.008726647f;
        radius = cam->m_FarClip / func_002974C0(ang);
        corners[0].x = 0.0f;
        corners[0].y = 0.0f;
        corners[0].z = 0.0f;
        cornerX = radius * func_002982F0(ang);
        cornerY = cornerX / aspect;
        corners[1].x = cornerX;
        corners[1].y = cornerY;
        depth = -cam->m_FarClip;
        corners[2].x = -cornerX;
        corners[2].y = cornerY;
        corners[3].x = cornerX;
        corners[3].y = -cornerY;
        corners[4].x = -cornerX;
        corners[4].y = -cornerY;
        corners[1].z = depth;
        corners[2].z = depth;
        corners[3].z = depth;
        corners[4].z = depth;
    } else {
        boxX = halfX * 0.5f;
        boxY = halfY * 0.5f;
        corners[0].x = 0.0f;
        corners[0].y = 0.0f;
        corners[0].z = 0.0f;
        corners[1].x = boxX;
        corners[1].y = boxY;
        boxZ = -cam->m_FarClip;
        corners[2].x = -boxX;
        corners[2].y = boxY;
        corners[3].x = boxX;
        corners[3].y = -boxY;
        corners[4].x = -boxX;
        corners[4].y = -boxY;
        corners[1].z = boxZ;
        corners[2].z = boxZ;
        corners[3].z = boxZ;
        corners[4].z = boxZ;
        radius = func_00298470(boxX * boxX + boxY * boxY + boxZ * boxZ);
    }
    func_0020F908(cam->m_mfViewOrient, corners, cam->m_vTCorners, 5);
    func_0026F080(cam->m_ViewVolume, &cam->m_vTCorners[0], &cam->m_vTCorners[1], &cam->m_vTCorners[2],
                  &cam->m_vTCorners[3], &cam->m_vTCorners[4]);
    func_0026B9E0(&cam->m_ViewMin, 5, cam->m_vTCorners);
    r15 = radius * 1.5f;
    cam->m_AnimMin.x = cam->m_vTCorners[0].x - radius;
    cam->m_AnimMax.x = cam->m_vTCorners[0].x + radius;
    cam->m_AnimMin.y = cam->m_vTCorners[0].y - r15;
    cam->m_AnimMax.y = cam->m_vTCorners[0].y + r15;
    cam->m_AnimMin.z = cam->m_vTCorners[0].z - radius;
    cam->m_AnimMax.z = cam->m_vTCorners[0].z + radius;
    func_00211114(&cam->m_qViewAlignNoZ, cam->m_mfFaceScreen);
    func_002105A4(cam->m_mfFaceScreenYAxisUp, cam->m_vRotation.y);
    if (D_801198E8 != 0) {
        func_0029815C(mProj, &cam->m_OnScreenPerspNorm, 47.5f, D_800C2030 / D_800C2034, 16.0f, 7168.0f, 1.0f);
        func_0020F79C(mView);
        func_0020C890(m, mView, mProj);
        func_0020FC74(mFlip, -1.0f, 1.0f, 1.0f);
        func_0020C890(mOnScreen, m, mFlip);
        func_0020F7EC(mOnScreen);
        func_0020CC94(mOnScreen, &cam->m_mOnScreenProjection[D_800B6D1C]);
    }
    func_00282E4C(cam->m_SkySystem, cam);
    func_004169C8(cam->m_MenuPool);
    if (Cinema_PathPlaying() != 0) {
        cam->m_ViewParams.m_pCurrentRegion = (struct CGameRegion *) func_00220A1C(D_800F7078, &cam->m_ViewParams.m_vPos);
    }
    floor = (CFloor *) cam->m_ViewParams.m_pCurrentRegion;
    x = cam->m_ViewParams.m_vPos.x;
    y = cam->m_ViewParams.m_vPos.y;
    z = cam->m_ViewParams.m_vPos.z;
    height = cam->m_ViewParams.m_YPosOffset;
    cam->m_ViewParams.m_VisBits = D_800B6D00;
    if (floor != NULL && Cinema_PathPlaying() == 0) {
        above = ((y + height) - func_0026BE60(floor, x, z)) * 0.09765625f;
        if (above < 11.0f && above > 5.0f) {
            cam->m_ViewParams.m_VisBits = floor->m_VisBits;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        cam->m_ViewParams.m_VisBits = D_800B6D00;
    }
    if (cam->m_ViewParams.m_ZoomBlend > 0.0f) {
        cam->m_ViewParams.m_VisBits = D_800B6D00;
    }
}

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern s32 D_800B6D1C;          /* which of the two frame buffers is drawn */
extern Gfx D_800AB100[];        /* RDP setup display lists */
extern Gfx D_800AB130[];
extern CCamera D_801198F0;      /* the main camera (camera pool + 0x40) */
extern s32 D_801198E0;          /* the pool's active camera count */
typedef struct {
    /* 0x000 */ u8 _pad[0x110];
    /* 0x110 */ void *m_ColorImage;
} Frame;
typedef struct {
    /* 0x00 */ Frame *m_pFrame;
} FrameRef;
extern FrameRef D_800F7070;     /* the frame being drawn */
extern void *D_80130A78;        /* the z buffer */
void func_00207A54(void);
void func_00207FEC(s32 arg0);
s32 Cinema_PathPlaying(void);
u32 func_002843F4(void);        /* a cinematic's fog colour, 0xFF00 for none */
void func_002284E0(CCamera *cam);

#define GFX(dl, a, b) { Gfx *_g = (dl)++; _g->w0 = (a); _g->w1 = (b); }

static __inline__ void ResetPixelScale(CCamera *cam) {
    cam->m_PixelXScale = 1.0f;
    cam->m_PixelYScale = 1.0f;
}

/* The warp effect's scale, as an inline setter: its arguments are copied
 * into the parameters, and the scales are then read back from the camera
 * (reload turns the m_PixelXScale read into the ROM's mov.s). */
static __inline__ void SetPixelScale(CCamera *cam, f32 xs, f32 ys) {
    cam->m_PixelXScale = xs;
    cam->m_PixelYScale = ys;
}

/* Opens a camera's part of the frame's display list (LibTEngine's CCamera
 * DisplayListSetup): its projection, viewport, scissor and look-at; then,
 * unless it is the main camera with others active, clears the z buffer and
 * the view to the fog colour, with the fog from m_FogStart to 1000. */
void func_0027E000(CCamera *cam) {
    struct { u8 red, green, blue; } rgb;
    s16 ulx, uly, lrx, lry;
    s32 near, far, denom;
    u32 color;
    u32 fill;
    u32 red, green, blue;
    s16 vx, vy;
    s32 maxx, maxy;
    f32 sx, sy, width, height, x, y;
    Frame *buffer;
    s32 frame;

    {
        Gfx **setup = &D_800D8E24;

        GFX(*setup, 0xE7000000, 0);                         /* gDPPipeSync */
        GFX(*setup, 0xDE000000, (u32) D_800AB100);          /* gSPDisplayList */
        GFX(*setup, 0xDE000000, (u32) D_800AB130);
        ResetPixelScale(cam);
        SetPixelScale(cam, cam->m_WarpXScale, cam->m_WarpYScale);
        sx = cam->m_PixelXScale;
        sy = cam->m_PixelYScale;
        maxx = (u16) D_800C2030 - 1;
        maxy = (u16) D_800C2034 - 1;
        width = cam->m_Viewport.m_Width;
        height = cam->m_Viewport.m_Height;
        x = cam->m_Viewport.m_ScreenPosX;
        y = cam->m_Viewport.m_ScreenPosY;
        ulx = (s32) x;
        uly = (s32) y;
        vx = (s32) ((width + width) * sx);
        lrx = (s32) (x + width);
        lry = (s32) (y + height);
        vy = (s32) ((height + height) * sy);
        if (D_800C2030 - 1 < lrx) lrx = maxx;
        if (ulx < 0) ulx = 0;
        if (uly < 0) uly = 0;
        if (lry > D_800C2034 - 1) lry = maxy;
        buffer = D_800F7070.m_pFrame;
        frame = D_800B6D1C;
        GFX(*setup, 0xDA380007, (u32) &cam->m_mProjection[frame]);   /* gSPMatrix */
        GFX(*setup, 0xDB0E0000, cam->m_PerspNorm);                    /* gSPPerspNormalize */
        cam->m_vp[frame].vscale[0] = vx;
        cam->m_vp[frame].vscale[1] = vy;
        cam->m_vp[frame].vtrans[0] = (s32) ((vx * 0.25f + cam->m_Viewport.m_ScreenPosX) * 4.0f);
        cam->m_vp[frame].vtrans[1] = (s32) ((vy * 0.25f + cam->m_Viewport.m_ScreenPosY) * 4.0f);
        GFX(*setup, 0xDC080008, (u32) &cam->m_vp[frame]);             /* gSPViewport */
        GFX(*setup, 0xED000000 | ((s32) (ulx * 4.0f) & 0xFFF) << 12 | ((s32) (uly * 4.0f) & 0xFFF),
            ((s32) (lrx * 4.0f) & 0xFFF) << 12 | ((s32) (lry * 4.0f) & 0xFFF));   /* gDPSetScissor */
        GFX(*setup, 0xDC08000A, (u32) &cam->m_LookAt[frame]);                        /* gSPLookAtX */
        GFX(*setup, 0xDC08030A, (u32) &cam->m_LookAt[frame] + 0x10);                 /* gSPLookAtY */
        func_00207A54();
    }
    GFX(D_800D8E24, 0xE3001201, 0x2000);                    /* gDPSetTextureFilter(bilerp) */
    if (cam != &D_801198F0 || D_801198E0 == 0) {
        Gfx **clearDl = &D_800D8E24;

        func_00207FEC(0x14);
        GFX(*clearDl, 0xE7000000, 0);
        GFX(*clearDl, 0xE3000A01, 0x300000);                /* gDPSetCycleType(G_CYC_FILL) */
        GFX(*clearDl, 0xFF100000 | ((D_800C2030 - 1) & 0xFFF), (u32) D_80130A78);   /* colour image: z buffer */
        GFX(*clearDl, 0xE7000000, 0);
        GFX(*clearDl, 0xE3000A01, 0x300000);
        GFX(*clearDl, 0xF7000000, 0xFFFCFFFC);              /* gDPSetFillColor(max z) */
        {
            Gfx *cmd = *clearDl;

            GFX(cmd, 0xF6000000 | (lrx & 0x3FF) << 14 | (lry & 0x3FF) << 2,
                (ulx & 0x3FF) << 14 | (uly & 0x3FF) << 2);  /* gDPFillRectangle */
            *clearDl = cmd;
        }
        if (D_8011AAD8.cheatFlags & 0x4000) {
            rgb.red = 0;
            rgb.green = 0;
            rgb.blue = 0;
        } else {
            rgb.red = cam->m_FogColor[0] & 0xF8;
            rgb.green = cam->m_FogColor[1] & 0xF8;
            rgb.blue = cam->m_FogColor[2] & 0xF8;
        }
        if (Cinema_PathPlaying() != 0) {
            color = func_002843F4();
            if (color != 0xFF00) {
                rgb.blue = color >> 16;
                rgb.green = color >> 8;
                rgb.red = color;
            }
        }
        GFX(D_800D8E24, 0xD9FFFFFF, 0x10000);               /* gSPSetGeometryMode(G_FOG) */
        {
            Gfx *g = D_800D8E24++;                          /* gSPFogPosition(m_FogStart, 1000) */
            g->w0 = 0xDB080000;
            far = (u16) cam->m_FogStart;
            denom = 1000 - far;
            near = (500 - far) * 256;
            g->w1 = (u32) (0x1F400 / denom) << 16 | ((near / denom) & 0xFFFF);
        }
        {
            Gfx *g = D_800D8E24++;                          /* gDPSetFogColor */
            g->w0 = 0xF8000000;
            red = rgb.red;
            green = rgb.green;
            blue = rgb.blue;
            g->w1 = red << 24 | green << 16 | blue << 8 | 0xFF;
        }
        fill = ((red << 8) & 0xF800) | ((green << 3) & 0x7C0) | ((blue >> 2) & 0x3E);
        GFX(D_800D8E24, 0xFF100000 | ((D_800C2030 - 1) & 0xFFF), (u32) buffer->m_ColorImage);
        GFX(D_800D8E24, 0xF7000000, ((fill | 1) << 16 | 1) + (u16) fill);
        GFX(D_800D8E24, 0xF6000000 | (lrx & 0x3FF) << 14 | (lry & 0x3FF) << 2,
            (ulx & 0x3FF) << 14 | (uly & 0x3FF) << 2);
    }
    GFX(D_800D8E24, 0xE7000000, 0);
    GFX(D_800D8E24, 0xE3001801, 0);                         /* gDPSetAlphaDither */
    GFX(D_800D8E24, 0xE3001A01, 0x10);
    GFX(D_800D8E24, 0xE3000A01, 0x100000);                  /* gDPSetCycleType(G_CYC_2CYCLE) */
    if (cam != &D_801198F0) {
        func_002284E0(cam);
    }
    GFX(D_800D8E24, 0xE7000000, 0);
}
