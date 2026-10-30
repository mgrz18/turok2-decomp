/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00238660.c, func_00238660, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "structs/camera.h"

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
f32 func_00298470(f32 x);       /* square root */
void func_0027B320(s32 sprite, s32 frame, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 arg6);

/* A lens flare from the screen point (x, y): seven glares strung from it
 * through the centre of the screen, fading as the point leaves the centre.
 * With `sun` set the first glare is the sun itself (sprite 0x258), faded in
 * 35.2 pixels from the screen edges, and the flare does not fade with
 * distance. */
void func_002803E8(CCamera *cam, f32 x, f32 y, f32 alpha, s32 sun, s32 unused) {
    f32 sizes[7] = { 16.0f, 16.0f, 24.0f, 32.0f, 16.0f, 32.0f, 48.0f };
    s32 opacities[7] = { 24, 24, 24, 24, 24, 24, 24 };
    s32 colours[21] = {
        255, 150, 150, 255, 255, 150, 150, 255, 150, 150, 255, 255,
        150, 150, 255, 200, 150, 255, 255, 150, 255
    };
    f32 dx, dy, dist, dirX, dirY, t, size, radius, adx, clamped;
    Gfx *g;
    s32 i;

    if (alpha != 0.0f) {
        dx = (f32) (D_800C2030 / 2) - x;
        dy = (f32) (D_800C2034 / 2) - y;
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xE3001201;             /* gDPSetTextureFilter(bilerp) */
            g->w1 = 0x2000;
        }
        dist = func_00298470(dx * dx + dy * dy);
        if (sun == 0) {
            t = dist / func_00298470((f32) (D_800C2030 * D_800C2030) * 0.25f + (f32) (D_800C2034 * D_800C2034) * 0.25f);
            if (1.0f < t) {
                alpha = 0.0f;
            } else {
                alpha = alpha * (1.0f - t);
            }
        }
        dist = dist * 0.4f;
        if (dx < 0.0f) adx = -dx; else adx = dx;
        if (dy < 0.0f ? (-dy < adx) : (dy < adx)) {
            dirX = -1.0f;
            if (0.0f < dx) dirX = 1.0f;
            dirY = (dx < 0.0f) ? dy / -dx : dy / dx;
        } else {
            dirX = (dy < 0.0f) ? dx / -dy : dx / dy;
            dirY = -1.0f;
            if (0.0f < dy) dirY = 1.0f;
        }
        i = 0;
        if (sun) {
            f32 border, a, b, c, e, m, m2;

            border = 35.2f;
            a = (f32) D_800C2034 + border;
            b = (f32) D_800C2030 + border;
            c = x + border;
            e = y + border;
            m = a - y;
            if (!(m <= b - x)) m = b - x;
            if (!(m <= e)) m = e;
            radius = border;        /* the ROM keeps the border in radius's register */
            if (!(m <= c)) m = c;
            if (!(m <= radius)) m = radius;
            if (m < 0.0f) {
                alpha = 0.0f;
            } else {
                m2 = a - y;
                if (!(m2 <= b - x)) m2 = b - x;
                if (!(m2 <= e)) m2 = e;
                if (!(m2 <= c)) m2 = c;
                if (!(m2 <= radius)) m2 = radius;
                alpha = alpha * (m2 / border);
            }
            if (!(1.0f < alpha)) {
                clamped = 0.0f;
                if (!(alpha < 0.0f)) {
                    goto clamp;
                }
            } else {
            clamp:
                clamped = alpha;
                if (1.0f < clamped) clamped = 1.0f;
            }
            alpha = clamped;
            i = 0;
        }
        i = 0;
        do {
            x += dirX * dist;
            y += dirY * dist;
            if (sun != 0 && i == 0) {
                size = 2.0f;
            } else {
                size = sizes[i] * 0.03125f;
            }
            t = size * 0.55f;
            radius = t * 16.0f;
            if (!(-radius < x && x < (f32) D_800C2030 + radius && -radius < y && y < (f32) D_800C2034 + radius)) {
                return;
            }
            if (sun && i == 0) {
                g = D_800D8E24++;
                g->w0 = 0xFB000000;                     /* gDPSetEnvColor */
                g->w1 = (u32) (alpha * 255.0f) | 0xFFFFFF00;
                func_0027B320(0x258, 0, (s32) (x - radius), (s32) (y - radius), t, t, 1);
            } else {
                g = D_800D8E24++;
                g->w0 = 0xFB000000;
                g->w1 = (u8) colours[i * 3] << 24 | (u8) colours[i * 3 + 1] << 16 | (u8) colours[i * 3 + 2] << 8
                      | ((u32) (opacities[i] * alpha) & 0xFF);
                func_0027B320(0x259, 0, (s32) (x - radius), (s32) (y - radius), t, t, 1);
            }
            i++;
        } while (i < 7);
    }
}
