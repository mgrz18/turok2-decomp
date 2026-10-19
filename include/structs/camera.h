#ifndef STRUCTS_CAMERA_H
#define STRUCTS_CAMERA_H

/* The camera. Layout and names from LibTEngine's Turok 2 types
 * (CCamera.h, CCameraViewParams.h, CShake.h...), with these departures:
 *   - LibTEngine skips 0x42C and ends CCamera at 0xEBC with sizeof 0xEC0:
 *     both are libultra's Mtx (8-byte aligned in PR/gbi.h) lining up, so
 *     they are written out as _pad6 and _pad7 here (this Mtx is a plain
 *     byte array, which LibTEngine's CPlayerOverlay at 0x0C needs);
 *   - CMenuPool and CSkySystem are kept opaque at the sizes CCamera and
 *     CCameraPool leave for them.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "structs/game_object.h"
#include "structs/gbi.h"

/* Acclaim's intrusive list (CList__*): the head and tail element, and where
 * in an element its links are (the IntrusiveList of the Rage Wars decomp). */
typedef struct {
    /* 0x00 */ void *m_pHead;
    /* 0x04 */ void *m_pTail;
    /* 0x08 */ s32 m_LastOffset;
    /* 0x0C */ s32 m_NextOffset;
    /* 0x10 */ s32 m_nCount;
} CList; /* size 0x14 */

typedef struct {
    /* 0x00 */ CVector3 m_vNormal;
    /* 0x0C */ f32 m_Dist;
} CPlane; /* size 0x10 */

typedef struct {
    /* 0x00 */ s32 m_nType;
    /* 0x04 */ f32 m_RotY;
    /* 0x08 */ CVector3 m_vRotOffset;
    /* 0x14 */ CVector3 m_vPos;
    /* 0x20 */ f32 m_YPosOffset;
    /* 0x24 */ CQuaternion m_qGround;
    /* 0x34 */ struct CGameRegion *m_pCurrentRegion;
    /* 0x38 */ f32 m_ZoomBlend;
    /* 0x3C */ f32 m_ZoomFov;
    /* 0x40 */ u32 m_VisBits;
} CCameraViewParams; /* size 0x44 */

typedef struct {
    /* 0x00 */ f32 m_Width;
    /* 0x04 */ f32 m_Height;
    /* 0x08 */ f32 m_ScreenPosX;
    /* 0x0C */ f32 m_ScreenPosY;
} CCameraViewport; /* size 0x10 */

typedef struct {
    /* 0x00 */ s32 m_Type;
    /* 0x04 */ f32 m_Shake;
    /* 0x08 */ f32 m_LastShake;
    /* 0x0C */ f32 m_Amp;
    /* 0x10 */ f32 m_Angle;
} CShakeAxis; /* size 0x14 */

/* A camera shake source: position, radius and a wobble per axis. */
typedef struct CShake {
    /* 0x00 */ struct CShake *m_pLast;
    /* 0x04 */ struct CShake *m_pNext;
    /* 0x08 */ CVector3 m_vPos;
    /* 0x14 */ f32 m_Radius;
    /* 0x18 */ CShakeAxis m_XAxis;
    /* 0x2C */ CShakeAxis m_YAxis;
    /* 0x40 */ CShakeAxis m_ZAxis;
} CShake; /* size 0x54 */

/* A text that fades in and out over the view (level names, messages). */
typedef struct CCameraText {
    /* 0x00 */ struct CCameraText *m_pLast;
    /* 0x04 */ struct CCameraText *m_pNext;
    /* 0x08 */ f32 m_Time;
    /* 0x0C */ f32 m_DisplayTime;
    /* 0x10 */ f32 m_Scale;
    /* 0x14 */ s32 m_nMode;
    /* 0x18 */ s32 m_nAlpha;
    /* 0x1C */ s32 m_nId;
    /* 0x20 */ s32 m_bSmall;
    /* 0x24 */ char *m_pString;
    /* 0x28 */ f32 m_ModeTime;
    /* 0x2C */ f32 m_ModeDuration;
    /* 0x30 */ f32 m_PositionX;
    /* 0x34 */ f32 m_PositionY;
    /* 0x38 */ f32 m_ScaleX;
    /* 0x3C */ f32 m_ScaleY;
} CCameraText; /* size 0x40 */

typedef struct CCamera {
    /* 0x000 */ struct CCamera *m_pLast;
    /* 0x004 */ struct CCamera *m_pNext;
    /* 0x008 */ s32 m_Index;
    /* 0x00C */ f32 m_PixelXScale;
    /* 0x010 */ f32 m_PixelYScale;
    /* 0x014 */ s16 m_Mode;
    /* 0x016 */ s16 _pad0;
    /* 0x018 */ f32 m_Time;
    /* 0x01C */ s16 m_Stage;
    /* 0x01E */ s16 _pad1;
    /* 0x020 */ f32 m_StageTime;
    /* 0x024 */ CCameraViewParams m_ViewParams;
    /* 0x068 */ u16 m_PerspNorm;
    /* 0x06A */ s16 _pad2;
    /* 0x06C */ f32 m_Fov;
    /* 0x070 */ f32 m_AspectRatio;
    /* 0x074 */ f32 m_XScale;
    /* 0x078 */ f32 m_YScale;
    /* 0x07C */ f32 m_WaterWobbleXScale;
    /* 0x080 */ f32 m_WaterWobbleYScale;
    /* 0x084 */ f32 m_WaterWobbleXAngle;
    /* 0x088 */ f32 m_WaterWobbleYAngle;
    /* 0x08C */ f32 m_WaterWobbleXSpeed;
    /* 0x090 */ f32 m_WaterWobbleYSpeed;
    /* 0x094 */ CShake m_Shake;
    /* 0x0E8 */ f32 m_TremorAngle;
    /* 0x0EC */ f32 m_TremorHeight;
    /* 0x0F0 */ s32 m_WarpMode;
    /* 0x0F4 */ f32 m_WarpTime;
    /* 0x0F8 */ CVector3 m_vWarpPos;
    /* 0x104 */ f32 m_WarpXScale;
    /* 0x108 */ f32 m_WarpYScale;
    /* 0x10C */ f32 m_WarpSpin;
    /* 0x110 */ s32 m_Flags;
    /* 0x114 */ CVector3 m_vPos;
    /* 0x120 */ CVector3 m_vRotation;
    /* 0x12C */ CQuaternion m_qViewAlign;
    /* 0x13C */ CQuaternion m_qViewAlignNoZ;
    /* 0x14C */ CMtxF m_mfViewOrient;
    /* 0x18C */ CMtxF m_mfInverseViewOrient;
    /* 0x1CC */ CMtxF m_mfProjection;
    /* 0x20C */ CMtxF m_mfView;
    /* 0x24C */ CVector3 m_vTCorners[5];
    /* 0x288 */ CCameraViewport m_Viewport;
    /* 0x298 */ Vp m_vp[2];
    /* 0x2B8 */ Vp m_vpSecondary[2];
    /* 0x2D8 */ CPlane m_ViewVolume[6];
    /* 0x338 */ CVector3 m_ViewMin;
    /* 0x344 */ CVector3 m_ViewMax;
    /* 0x350 */ CVector3 m_AnimMin;
    /* 0x35C */ CVector3 m_AnimMax;
    /* 0x368 */ Mtx m_mProjection[2];
    /* 0x3E8 */ LookAt m_LookAt[2];
    /* 0x428 */ u16 m_OnScreenPerspNorm;
    /* 0x42A */ u16 _pad3;
    /* 0x42C */ s32 _pad6;                     /* aligns the Mtx below to 8, as libultra's Mtx is */
    /* 0x430 */ Mtx m_mOnScreenProjection[2];
    /* 0x4B0 */ LookAt m_mOnScreenLookAt[2];
    /* 0x4F0 */ s32 m_ScreenLeft;
    /* 0x4F4 */ s32 m_ScreenRight;
    /* 0x4F8 */ s32 m_ScreenTop;
    /* 0x4FC */ s32 m_ScreenBottom;
    /* 0x500 */ s32 m_nCurrentRegionSet;
    /* 0x504 */ f32 m_cRegionSetBlend;
    /* 0x508 */ u8 m_FogColor[4];
    /* 0x50C */ u8 m_LastFogColor[4];
    /* 0x510 */ f32 m_FarClip;
    /* 0x514 */ f32 m_LastFarClip;
    /* 0x518 */ f32 m_FieldOfView;
    /* 0x51C */ f32 m_LastFieldOfView;
    /* 0x520 */ f32 m_FlashTimer;
    /* 0x524 */ s32 m_FogStart;
    /* 0x528 */ s32 m_LastFogStart;
    /* 0x52C */ s32 m_FlashMode;
    /* 0x530 */ u8 m_Flash;
    /* 0x531 */ u8 m_FlashDec;
    /* 0x532 */ u8 m_MaxOpacity;
    /* 0x533 */ u8 m_AttackTime;
    /* 0x534 */ u8 m_SustainTime;
    /* 0x535 */ u8 m_DecayTime;
    /* 0x536 */ u8 m_FlashColor[4];
    /* 0x53A */ u8 m_UnderWater;
    /* 0x53B */ u8 _pad5;
    /* 0x53C */ u8 m_MenuPool[0x18];             /* CMenuPool */
    /* 0x554 */ s32 _pad4;
    /* 0x558 */ u8 m_SkySystem[0x8D0];           /* CSkySystem */
    /* 0xE28 */ CList m_ActiveTextList;
    /* 0xE3C */ CMtxF m_mfFaceScreen;
    /* 0xE7C */ CMtxF m_mfFaceScreenYAxisUp;
    /* 0xEBC */ s32 _pad7;                     /* to a multiple of 8, again for Mtx */
} CCamera; /* size 0xEC0 */

/* All cameras: the free and active lists, the main camera, the fading
 * texts and the shake sources. */
typedef struct {
    /* 0x0000 */ struct CMemEntry *m_pmeCameras;
    /* 0x0004 */ struct CCamera *m_Cameras;
    /* 0x0008 */ s32 m_nCameras;
    /* 0x000C */ CList m_FreeList;
    /* 0x0020 */ CList m_ActiveList;
    /* 0x0034 */ s32 m_ActiveLastFrame;
    /* 0x0038 */ s32 m_bActiveChanged;
    /* 0x003C */ s32 _pad0;
    /* 0x0040 */ CCamera m_MainCamera;
    /* 0x0F00 */ struct CMemEntry *m_pmeTexts;
    /* 0x0F04 */ struct CCameraText *m_Texts;
    /* 0x0F08 */ s32 m_nTexts;
    /* 0x0F0C */ CList m_FreeTextList;
    /* 0x0F20 */ CShake m_ShakeVars[8];
    /* 0x11C0 */ CList m_ShakeFreeList;
    /* 0x11D4 */ CList m_ShakeActiveList;
    /* 0x11E8 */ s32 m_bOnlyUseMainCameraForSound;
    /* 0x11EC */ s32 _pad1;
} CCameraPool; /* size 0x11F0 */

#endif
