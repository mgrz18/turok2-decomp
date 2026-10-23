typedef int s32;
typedef unsigned int u32;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

extern Gfx *D_800D8E24;         /* display list head */
extern u32 D_800AB884;          /* current render mode index */
extern s32 D_800AB890;          /* anti-aliasing enabled */
extern u32 D_800AB8A8;          /* extra blender flags */
extern u32 D_800AB8AC;          /* extra blender flags (z-buffered modes) */

/* gDPSetRenderMode(D_800D8E24++, mode) */
#define SET_RENDER_MODE(mode)                     \
    {                                             \
        Gfx *_g = D_800D8E24++;                   \
        _g->w0 = 0xE200001C;                      \
        _g->w1 = (mode);                          \
    }

/* Switches the RDP render mode; returns 0 when it was already active. */
s32 func_00207FEC(u32 mode) {
    if (D_800AB884 == mode) {
        return 0;
    }
    D_800AB884 = mode;
    switch (mode) {
    case 0:
        if (D_800AB890) {
            SET_RENDER_MODE(0xC1849D8);
        } else {
            SET_RENDER_MODE(0xC184A50);
        }
        break;
    case 1:
        if (D_800AB890) {
            SET_RENDER_MODE(D_800AB8AC | 0x1049D8);
        } else {
            SET_RENDER_MODE(D_800AB8AC | 0x104A50);
        }
        break;
    case 2:
        SET_RENDER_MODE(0xC184A50);
        break;
    case 3:
        SET_RENDER_MODE(0xC184B50);
        break;
    case 4:
        if (D_800AB890) {
            SET_RENDER_MODE(D_800AB8AC | 0x104DD8);
        } else {
            SET_RENDER_MODE(D_800AB8AC | 0x104E50);
        }
        break;
    case 5:
        if (D_800AB890) {
            SET_RENDER_MODE(D_800AB8AC | 0x1041F8);
        } else {
            SET_RENDER_MODE(D_800AB8AC | 0x1041F0);
        }
        break;
    case 6:
        if (D_800AB890) {
            SET_RENDER_MODE(D_800AB8AC | 0x113078);
        } else {
            SET_RENDER_MODE(D_800AB8AC | 0x113078);
        }
        break;
    case 7:
        SET_RENDER_MODE(D_800AB8AC | 0x112E10);
        break;
    case 8:
        if (D_800AB890) {
            SET_RENDER_MODE(D_800AB8AC | 0x112478);
        } else {
            SET_RENDER_MODE(D_800AB8AC | 0x112478);
        }
        break;
    case 9:
        SET_RENDER_MODE(D_800AB8AC | 0x112438);
        break;
    case 10:
        if (D_800AB890) {
            SET_RENDER_MODE(D_800AB8AC | 0x112078);
        } else {
            SET_RENDER_MODE(D_800AB8AC | 0x112230);
        }
        break;
    case 11:
        SET_RENDER_MODE(D_800AB8AC | 0x112038);
        break;
    case 12:
        if (D_800AB890) {
            SET_RENDER_MODE(0x5045D8);
        } else {
            SET_RENDER_MODE(0x4045D0);
        }
        break;
    case 13:
        SET_RENDER_MODE(0x504B50);
        break;
    case 14:
        if (D_800AB890) {
            SET_RENDER_MODE(0x5049D8);
        } else {
            SET_RENDER_MODE(0x504A50);
        }
        break;
    case 15:
        SET_RENDER_MODE(0x504E50);
        break;
    case 16:
        SET_RENDER_MODE(0x504B53);
        break;
    case 17:
        if (D_800AB890) {
            SET_RENDER_MODE(0x553078);
        } else {
            SET_RENDER_MODE(0x553078);
        }
        break;
    case 18:
        if (D_800AB890) {
            SET_RENDER_MODE(0x553048);
        } else {
            SET_RENDER_MODE(0xF0A7008);
        }
        break;
    case 19:
        SET_RENDER_MODE(0x504340);
        break;
    case 20:
        SET_RENDER_MODE(0);
        break;
    case 21:
        SET_RENDER_MODE(0x504240);
        break;
    case 22:
        SET_RENDER_MODE(0xFA54040);
        break;
    case 23:
        SET_RENDER_MODE(D_800AB8AC | 0x104B50);
        break;
    case 24:
        SET_RENDER_MODE(D_800AB8AC | 0x104E50);
        break;
    case 25:
        SET_RENDER_MODE(0xC184240);
        break;
    case 26:
        SET_RENDER_MODE(0xC184340);
        break;
    case 27:
        SET_RENDER_MODE(0xF0A4000);
        break;
    case 28:
        if (D_800AB890) {
            SET_RENDER_MODE(0x507048);
        } else {
            SET_RENDER_MODE(0x507040);
        }
        break;
    case 29:
        SET_RENDER_MODE(D_800AB8AC | 0x104F50);
        break;
    case 30:
        SET_RENDER_MODE(D_800AB8A8 | 0x104F50);
        break;
    }
    return 1;
}
