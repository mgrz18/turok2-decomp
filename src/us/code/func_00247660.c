#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u16 zoomModel;       /* the scope model shown when zoomed, 0 = none */
} WeaponDef;

/* The scope model raised to the view (see func_00247170). */
typedef struct {
    /* 0x00 */ s32 current;
    /* 0x04 */ s32 requested;
    /* 0x08 */ u8 pad8[0xB4 - 8];
    /* 0xB4 */ s32 state;           /* 0 hidden, 1 raising, 2 up, 3 lowering */
    /* 0xB8 */ f32 time;
} HudModel; /* size 0xBC */

/* A player, as far as the sniper zoom goes. */
typedef struct {
    /* 0x000 */ u8 pad0[0x524];
    /* 0x524 */ s32 alive;
    /* 0x528 */ u8 pad528[0x996 - 0x528];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ u32 buttons;
    /* 0xA68 */ u32 pressed;
    /* 0xA6C */ u8 padA6C[0xB8C - 0xA6C];
    /* 0xB8C */ s32 sniperMode;     /* 0 off, 1 on, 2 zooming */
    /* 0xB90 */ f32 zoom;
    /* 0xB94 */ f32 targetZoom;
    /* 0xB98 */ u8 padB98[0xC10 - 0xB98];
    /* 0xC10 */ HudModel scope;
} ZoomPlayer;

extern WeaponDef *D_800AF618[];
extern s32 D_8011AAA0;

void func_00247170(HudModel *m, ZoomPlayer *owner);
void func_002117DC(f32 *value, f32 target, f32 rate);

/* Starts lowering the scope model if it is up or being raised. */
static __inline__ void LowerScope(HudModel *m) {
    if (m->state != 0 && m->state != 3) {
        m->state = 3;
    }
}

/* Shows `model` as the scope, raising it again if it is already that one. */
static __inline__ void RaiseScope(HudModel *m, s32 model) {
    if (m->requested == model) {
        m->state = 1;
    }
    m->requested = model;
}

/* Sniper zoom: toggled by button 0x100 on a weapon with a scope, then
 * adjusted by 8 (in) and 4 (out) while 0x10 is held. */
void func_00247660(ZoomPlayer *p) {
    f32 target;
    f32 t;
    u16 model;

    target = 0.0f;
    if ((s16) D_800AF618[p->weapon]->zoomModel <= 0 || p->sniperMode == 0) {
        p->sniperMode = 0;
        LowerScope(&p->scope);
    }
    if (p->alive != 0) {
        if ((p->pressed & 0x100) && (p->pressed & 0x100) == (p->buttons & 0x100)) {
            if (p->sniperMode != 0) {
                p->sniperMode = 0;
                LowerScope(&p->scope);
            } else if (D_800AF618[p->weapon]->zoomModel != 0 && (D_8011AAA0 == 0 || !(p->buttons & 0x20))) {
                p->sniperMode = 1;
                if (p->weapon == 0x1B) {
                    p->targetZoom = 0.7f;
                } else {
                    p->targetZoom = 0.2f;
                }
                model = D_800AF618[p->weapon]->zoomModel;
                RaiseScope(&p->scope, model);
            }
        }
        if (p->sniperMode != 0) {
            target = p->targetZoom;
            p->sniperMode = 1;
            if (p->buttons & 0x10) {
                if (p->buttons & 8) {
                    t = target + 0.025f;
                    if (t > 1.0f) {
                        t = 1.0f;
                    }
                    target = t;
                    p->sniperMode = 2;
                }
                if (p->buttons & 4) {
                    t = target - 0.025f;
                    if (t < 0.2f) {
                        t = 0.2f;
                    }
                    target = t;
                    p->sniperMode = 2;
                }
            }
        }
    }
    p->targetZoom = target;
    func_00247170(&p->scope, p);
    if (p->scope.state != 1) {
        func_002117DC(&p->zoom, target, 0.5f);
        if (target == 0.0f && p->zoom < 0.01f) {
            p->zoom = 0.0f;
        }
    }
}
