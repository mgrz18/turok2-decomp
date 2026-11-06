/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_8.CPP, func_80236874_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/camera.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/render.h"

extern f32 gSplitScreenMarginBottom;          /* split screen margins: bottom, */
extern f32 gSplitScreenMarginTop;          /* top, */
extern f32 gSplitScreenMarginLeft;          /* left (not yet a symbol of its own: gSplitScreenMarginTop + 4 in the asm) */
extern f32 gSplitScreenMarginRight;          /* right */
extern s32 gDefaultVisBits[];        /* the visibility bits outside any region */

/* gCameraVisBits seen as a structure: the loop ORs into it through one. */
typedef struct {
    /* 0x0 */ u32 m_Bits;
} CVisBits;

u32 CShakeAxis__Advance(CShakeAxis *axis);
void CList__Remove(CList *list, void *item);
void CList__AddHead(CList *list, void *item);
void CCamera__MoveViewport(CCamera *cam, f32 width, f32 height, f32 x, f32 y);
void CCamera__Advance(CCamera *cam);

/* A camera's m_pNext, read as a plain word (not as a structure field, which
 * would let GCC move the read above the store to gCameraVisBits before it). */
#define NEXT_CAMERA(c) (*(CCamera **) ((u8 *) (c) + 4))

/* Advances the camera pool (LibTEngine's CCameraPool Advance): returns the
 * finished shakes to the free list, lays out the viewports of the active
 * cameras for one to four players, advances the main camera and each active
 * one, gathering their view boxes and visibility bits, and counts down
 * m_bActiveChanged after the number of active cameras changes. */
void CCameraPool__Advance(CCameraPool *pool) {
    f32 halfWidth, halfHeight, width, height;
    f32 top, left, right, bottom;
    f32 splitX, splitY;
    CShake *shake;
    CShake *nextShake;
    u32 xDone, yDone, zDone;
    s32 finished;
    s32 timer;
    CCamera *view;
    CBox *clip;
    CBox *bounds;
    CVisBits *flags;
    f32 v;

    right = (f32) gScreenWidth;
    left = 0.0f;
    bottom = (f32) gScreenHeight;
    top = 0.0f;
    if ((u32) pool->m_ActiveList.m_nCount >= 2U) {
        top = gSplitScreenMarginTop;
        bottom -= gSplitScreenMarginBottom;
        left = gSplitScreenMarginLeft;
        right -= gSplitScreenMarginRight;
    }
    width = right - left;
    halfWidth = width * 0.5f;
    height = bottom - top;
    halfHeight = height * 0.5f;
    shake = (CShake *) gEngineApp_m_Game_m_CameraPool.m_ShakeActiveList.m_pHead;
    view = &pool->m_MainCamera;
    if (shake != NULL) {
        do {
            nextShake = shake->m_pNext;
            xDone = CShakeAxis__Advance(&shake->m_XAxis);
            yDone = CShakeAxis__Advance(&shake->m_YAxis);
            zDone = CShakeAxis__Advance(&shake->m_ZAxis);
            finished = 0;
            if (xDone != 0 && yDone != 0) {
                finished = zDone > 0U;
            }
            if (finished != 0) {
                CList__Remove(&pool->m_ShakeActiveList, shake);
                CList__AddHead(&pool->m_ShakeFreeList, shake);
            }
            shake = nextShake;
            view = &pool->m_MainCamera;
        } while (shake != NULL);
    }
    view->m_Viewport.m_Width = (f32) gScreenWidth;
    view->m_Viewport.m_Height = (f32) gScreenHeight;
    view->m_Viewport.m_ScreenPosX = 0;
    view->m_Viewport.m_ScreenPosY = 0;
    view = (CCamera *) pool->m_ActiveList.m_pHead;
    switch ((u32) pool->m_ActiveList.m_nCount) {
    case 1:
        CCamera__MoveViewport(view, width, height, 0.0f, 0.0f);
        break;
    case 2:
        if (gEngineApp_m_Settings.unk1B == 0) {
            CCamera__MoveViewport(view, width, halfHeight, left, top);
            view = view->m_pNext;
            CCamera__MoveViewport(view, width, halfHeight, left, top + halfHeight);
        } else {
            CCamera__MoveViewport(view, halfWidth, height, left, top);
            view = view->m_pNext;
            CCamera__MoveViewport(view, halfWidth, height, left + halfWidth, top);
        }
        break;
    case 3:
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, top);
        view = view->m_pNext;
        CCamera__MoveViewport(view, halfWidth, halfHeight, left + halfWidth, top);
        view = view->m_pNext;
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, top + halfHeight);
        break;
    case 4:
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, top);
        view = view->m_pNext;
        splitX = left + halfWidth;
        CCamera__MoveViewport(view, halfWidth, halfHeight, splitX, top);
        view = view->m_pNext;
        splitY = top + halfHeight;
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, splitY);
        view = view->m_pNext;
        CCamera__MoveViewport(view, halfWidth, halfHeight, splitX, splitY);
        break;
    }
    clip = &gCameraViewBox;
    *clip = *(CBox *) &pool->m_MainCamera.m_ViewMin;
    bounds = &gCameraVisibleBox;
    *bounds = *(CBox *) &pool->m_MainCamera.m_AnimMin;
    gCameraVisBits = gDefaultVisBits[0];
    CCamera__Advance(&pool->m_MainCamera);
    view = (CCamera *) pool->m_ActiveList.m_pHead;
    if (view != NULL) {
        CCamera__Advance(view);
        *clip = *(CBox *) &view->m_ViewMin;
        *bounds = *(CBox *) &view->m_AnimMin;
        gCameraVisBits = view->m_ViewParams.m_VisBits;
        view = NEXT_CAMERA(view);
        if (view != NULL) {
            flags = (CVisBits *) &gCameraVisBits;
            do {
                CCamera__Advance(view);
                v = view->m_ViewMin.x;
                if (!(v <= clip->m_vMin.x)) v = clip->m_vMin.x;
                clip->m_vMin.x = v;
                v = view->m_ViewMin.y;
                if (!(v <= clip->m_vMin.y)) v = clip->m_vMin.y;
                clip->m_vMin.y = v;
                v = view->m_ViewMin.z;
                if (!(v <= clip->m_vMin.z)) v = clip->m_vMin.z;
                clip->m_vMin.z = v;
                v = view->m_ViewMax.x;
                if (!(clip->m_vMax.x <= v)) v = clip->m_vMax.x;
                clip->m_vMax.x = v;
                v = view->m_ViewMax.y;
                if (!(clip->m_vMax.y <= v)) v = clip->m_vMax.y;
                clip->m_vMax.y = v;
                v = view->m_ViewMax.z;
                if (!(clip->m_vMax.z <= v)) v = clip->m_vMax.z;
                clip->m_vMax.z = v;
                v = view->m_AnimMin.x;
                if (!(v <= bounds->m_vMin.x)) v = bounds->m_vMin.x;
                bounds->m_vMin.x = v;
                v = view->m_AnimMin.y;
                if (!(v <= bounds->m_vMin.y)) v = bounds->m_vMin.y;
                bounds->m_vMin.y = v;
                v = view->m_AnimMin.z;
                if (!(v <= bounds->m_vMin.z)) v = bounds->m_vMin.z;
                bounds->m_vMin.z = v;
                v = view->m_AnimMax.x;
                if (!(bounds->m_vMax.x <= v)) v = bounds->m_vMax.x;
                bounds->m_vMax.x = v;
                v = view->m_AnimMax.y;
                if (!(bounds->m_vMax.y <= v)) v = bounds->m_vMax.y;
                bounds->m_vMax.y = v;
                v = view->m_AnimMax.z;
                if (!(bounds->m_vMax.z <= v)) v = bounds->m_vMax.z;
                bounds->m_vMax.z = v;
                flags->m_Bits |= view->m_ViewParams.m_VisBits;
                view = view->m_pNext;
            } while (view != NULL);
        }
    }
    if (pool->m_ActiveLastFrame != pool->m_ActiveList.m_nCount) {
        pool->m_bActiveChanged = 2;
        pool->m_ActiveLastFrame = pool->m_ActiveList.m_nCount;
        return;
    }
    timer = pool->m_bActiveChanged;
    if (timer != 0) {
        pool->m_bActiveChanged = timer - 1;
    }
}
