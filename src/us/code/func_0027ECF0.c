/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_8.CPP, func_80236874_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/camera.h"

/* A bounding box. */
typedef struct {
    /* 0x00 */ CVector3 m_Min;
    /* 0x0C */ CVector3 m_Max;
} CBox;

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern f32 D_800B6CC4;          /* split screen margins: bottom, */
extern f32 D_800B6CC8;          /* top, */
extern f32 D_800B6CCC;          /* left (not yet a symbol of its own: D_800B6CC8 + 4 in the asm) */
extern f32 D_800B6CD0;          /* right */
extern u8 D_8011AAF3;           /* two players split side by side, not one above the other */
extern s32 D_800B6D00[];        /* the visibility bits outside any region */
extern CCameraPool D_801198B0;  /* the camera pool */
extern CBox D_800F5A28;         /* the view boxes of all the active cameras together */
extern CBox D_800F5A10;         /* and their widened (animation) boxes */
extern u32 D_800F5A40;          /* and their visibility bits */

/* D_800F5A40 seen as a structure: the loop ORs into it through one. */
typedef struct {
    /* 0x0 */ u32 m_Bits;
} CVisBits;

u32 func_0027BD30(CShakeAxis *axis);
void CList__Remove(CList *list, void *item);
void CList__AddHead(CList *list, void *item);
void func_0027EA88(CCamera *cam, f32 width, f32 height, f32 x, f32 y);
void func_0027D160(CCamera *cam);

/* A camera's m_pNext, read as a plain word (not as a structure field, which
 * would let GCC move the read above the store to D_800F5A40 before it). */
#define NEXT_CAMERA(c) (*(CCamera **) ((u8 *) (c) + 4))

/* Advances the camera pool (LibTEngine's CCameraPool Advance): returns the
 * finished shakes to the free list, lays out the viewports of the active
 * cameras for one to four players, advances the main camera and each active
 * one, gathering their view boxes and visibility bits, and counts down
 * m_bActiveChanged after the number of active cameras changes. */
void func_0027ECF0(CCameraPool *pool) {
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

    right = (f32) D_800C2030;
    left = 0.0f;
    bottom = (f32) D_800C2034;
    top = 0.0f;
    if ((u32) pool->m_ActiveList.m_nCount >= 2U) {
        top = D_800B6CC8;
        bottom -= D_800B6CC4;
        left = D_800B6CCC;
        right -= D_800B6CD0;
    }
    width = right - left;
    halfWidth = width * 0.5f;
    height = bottom - top;
    halfHeight = height * 0.5f;
    shake = (CShake *) D_801198B0.m_ShakeActiveList.m_pHead;
    view = &pool->m_MainCamera;
    if (shake != NULL) {
        do {
            nextShake = shake->m_pNext;
            xDone = func_0027BD30(&shake->m_XAxis);
            yDone = func_0027BD30(&shake->m_YAxis);
            zDone = func_0027BD30(&shake->m_ZAxis);
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
    view->m_Viewport.m_Width = (f32) D_800C2030;
    view->m_Viewport.m_Height = (f32) D_800C2034;
    view->m_Viewport.m_ScreenPosX = 0;
    view->m_Viewport.m_ScreenPosY = 0;
    view = (CCamera *) pool->m_ActiveList.m_pHead;
    switch ((u32) pool->m_ActiveList.m_nCount) {
    case 1:
        func_0027EA88(view, width, height, 0.0f, 0.0f);
        break;
    case 2:
        if (D_8011AAF3 == 0) {
            func_0027EA88(view, width, halfHeight, left, top);
            view = view->m_pNext;
            func_0027EA88(view, width, halfHeight, left, top + halfHeight);
        } else {
            func_0027EA88(view, halfWidth, height, left, top);
            view = view->m_pNext;
            func_0027EA88(view, halfWidth, height, left + halfWidth, top);
        }
        break;
    case 3:
        func_0027EA88(view, halfWidth, halfHeight, left, top);
        view = view->m_pNext;
        func_0027EA88(view, halfWidth, halfHeight, left + halfWidth, top);
        view = view->m_pNext;
        func_0027EA88(view, halfWidth, halfHeight, left, top + halfHeight);
        break;
    case 4:
        func_0027EA88(view, halfWidth, halfHeight, left, top);
        view = view->m_pNext;
        splitX = left + halfWidth;
        func_0027EA88(view, halfWidth, halfHeight, splitX, top);
        view = view->m_pNext;
        splitY = top + halfHeight;
        func_0027EA88(view, halfWidth, halfHeight, left, splitY);
        view = view->m_pNext;
        func_0027EA88(view, halfWidth, halfHeight, splitX, splitY);
        break;
    }
    clip = &D_800F5A28;
    *clip = *(CBox *) &pool->m_MainCamera.m_ViewMin;
    bounds = &D_800F5A10;
    *bounds = *(CBox *) &pool->m_MainCamera.m_AnimMin;
    D_800F5A40 = D_800B6D00[0];
    func_0027D160(&pool->m_MainCamera);
    view = (CCamera *) pool->m_ActiveList.m_pHead;
    if (view != NULL) {
        func_0027D160(view);
        *clip = *(CBox *) &view->m_ViewMin;
        *bounds = *(CBox *) &view->m_AnimMin;
        D_800F5A40 = view->m_ViewParams.m_VisBits;
        view = NEXT_CAMERA(view);
        if (view != NULL) {
            flags = (CVisBits *) &D_800F5A40;
            do {
                func_0027D160(view);
                v = view->m_ViewMin.x;
                if (!(v <= clip->m_Min.x)) v = clip->m_Min.x;
                clip->m_Min.x = v;
                v = view->m_ViewMin.y;
                if (!(v <= clip->m_Min.y)) v = clip->m_Min.y;
                clip->m_Min.y = v;
                v = view->m_ViewMin.z;
                if (!(v <= clip->m_Min.z)) v = clip->m_Min.z;
                clip->m_Min.z = v;
                v = view->m_ViewMax.x;
                if (!(clip->m_Max.x <= v)) v = clip->m_Max.x;
                clip->m_Max.x = v;
                v = view->m_ViewMax.y;
                if (!(clip->m_Max.y <= v)) v = clip->m_Max.y;
                clip->m_Max.y = v;
                v = view->m_ViewMax.z;
                if (!(clip->m_Max.z <= v)) v = clip->m_Max.z;
                clip->m_Max.z = v;
                v = view->m_AnimMin.x;
                if (!(v <= bounds->m_Min.x)) v = bounds->m_Min.x;
                bounds->m_Min.x = v;
                v = view->m_AnimMin.y;
                if (!(v <= bounds->m_Min.y)) v = bounds->m_Min.y;
                bounds->m_Min.y = v;
                v = view->m_AnimMin.z;
                if (!(v <= bounds->m_Min.z)) v = bounds->m_Min.z;
                bounds->m_Min.z = v;
                v = view->m_AnimMax.x;
                if (!(bounds->m_Max.x <= v)) v = bounds->m_Max.x;
                bounds->m_Max.x = v;
                v = view->m_AnimMax.y;
                if (!(bounds->m_Max.y <= v)) v = bounds->m_Max.y;
                bounds->m_Max.y = v;
                v = view->m_AnimMax.z;
                if (!(bounds->m_Max.z <= v)) v = bounds->m_Max.z;
                bounds->m_Max.z = v;
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
