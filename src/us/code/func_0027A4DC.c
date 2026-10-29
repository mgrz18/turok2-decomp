/* func_0027A4DC and func_0027A7A0, one source file: the file's .rodata opens
 * at 0x800A83F0 with func_0027A4DC's 4.0f, and func_0027A7A0's doubles
 * (2^32 for unsigned-to-float) are 8-aligned from there; from its own
 * start, 0x800A83F4, they would land 4 bytes off. func_0027A4DC is the
 * Rage Wars import, its 4.0f now a literal rather than an extern. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94_3.CPP, func_802AA1AC_de, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

/* One block of a tiled image: its size, then its 16-bit texels (and, for an
 * image with an opacity mask, the 4-bit mask after them). */
typedef struct {
    /* 0x0 */ u32 m_Width;
    /* 0x4 */ u32 m_Height;
    /* 0x8 */ u16 m_Pixels[1];
} TileBlock;

/* A tiled image: columns x rows blocks, row by row. */
typedef struct {
    /* 0x0 */ u16 m_nColumns;
    /* 0x2 */ u16 m_nRows;
    /* 0x4 */ u32 _pad;
    /* 0x8 */ TileBlock m_Blocks[1];
} TileGrid;

void func_0027AE44(void *table, s32 frame, TileGrid **pgrid, u8 *popaque);
void func_0027A4DC(void *pTexture, f32 XPos, f32 YPos, int TextureWidth, int TextureHeight, f32 ScaleX, f32 ScaleY);
void func_0027A2C4(void *pTexture, int XPos, int YPos, int TextureWidth, int TextureHeight);
void func_00279528(void *pTexture, void *pOpacity, f32 XPos, f32 YPos, int TextureWidth, int TextureHeight, f32 ScaleX, f32 ScaleY);
void func_00279158(void *pTexture, void *pOpacity, int XPos, int YPos, int TextureWidth, int TextureHeight);
void func_0020565C(s32, void *);

/* The next block: past the data of this one (`size` bytes), then its header. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_5.c, func_802A9EE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
void func_0027A4DC(void *pTexture, f32 XPos, f32 YPos, int TextureWidth, int TextureHeight, f32 ScaleX, f32 ScaleY) {
    int dsdx;
    int dtdy;
    int xl, yl, xh, yh;
    dsdx = 1 << 10;
    if (ScaleX != 0.0f) {
        dsdx = dsdx / ScaleX;
    }
    dtdy = 1 << 10;
    if (ScaleY != 0.0f) {
        dtdy = dtdy / ScaleY;
    }
    xl = XPos * 4.0f;
    xh = (XPos + TextureWidth * ScaleX) * 4.0f;
    yl = YPos * 4.0f;
    yh = (YPos + TextureHeight * ScaleY) * 4.0f;
    { { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((unsigned int)((u32)pTexture)); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)((((((TextureWidth*TextureHeight+3)>>2)-1)<2047?(((TextureWidth*TextureHeight+3)>>2)-1):2047)))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)((((1<<11)+(1>(TextureWidth/16)?1:(TextureWidth/16))-1)/(1>(TextureWidth/16)?1:(TextureWidth/16))))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)(((((TextureWidth>>1)+7)>>3)))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; }
    { Gfx *g=D_800D8E24++; { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xE4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(xh) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(yh) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(xl) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(yl) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)(0)); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)((((u32)((dsdx)) & 0xFFFF) << ((16)))|(((u32)((dtdy)) & 0xFFFF) << ((0))))); }; }
}

#define NEXT_BLOCK(b, size)                                 \
    b = (TileBlock *) ((u8 *) (b) + (size));                \
    b = (TileBlock *) ((u8 *) (b) + 8)

static __inline__ void DrawScaled(TileGrid *grid, f32 x, f32 y, f32 scaleX, f32 scaleY) {
    TileBlock *block;
    u32 width, height;
    f32 fx, fy, col, row;

    fy = y;
    block = grid->m_Blocks;
    height = 0;
    row = grid->m_nRows;
    while (row-- != 0.0f) {
        col = grid->m_nColumns;
        fx = x;
        while (col-- != 0.0f) {
            height = block->m_Height;
            width = block->m_Width;
            func_0027A4DC(block->m_Pixels, fx, fy, width, height, scaleX, scaleY);
            fx += (f32) width * scaleX;
            NEXT_BLOCK(block, (width * height) >> 1);
        }
        fy += (f32) height * scaleY;
    }
}

static __inline__ void Draw(TileGrid *grid, s16 x, s32 y) {
    TileBlock *block;
    u32 width, height;
    s32 ix, iy;
    s32 i, j;

    iy = y;
    block = grid->m_Blocks;
    height = 0;
    j = grid->m_nRows;
    while (--j != -1) {
        i = grid->m_nColumns;
        ix = x;
        while (--i != -1) {
            width = block->m_Width;
            height = block->m_Height;
            func_0027A2C4(block->m_Pixels, ix, (s16) iy, width, height);
            ix += width;
            NEXT_BLOCK(block, (width * height) >> 1);
        }
        iy += height;
    }
}

static __inline__ void DrawScaledMasked(TileGrid *grid, f32 x, f32 y, f32 scaleX, f32 scaleY) {
    TileBlock *block;
    u32 width, height, size;
    f32 fx, fy, col, row;

    fy = y;
    block = grid->m_Blocks;
    height = 0;
    row = grid->m_nRows;
    while (row-- != 0.0f) {
        col = grid->m_nColumns;
        fx = x;
        while (col-- != 0.0f) {
            height = block->m_Height;
            width = block->m_Width;
            size = width * height;
            func_00279528(block->m_Pixels, block->m_Pixels + size, fx, fy, width, height, scaleX, scaleY);
            fx += (f32) width * scaleX;
            NEXT_BLOCK(block, size * 2 + (size >> 1));
        }
        fy += (f32) height * scaleY;
    }
}

static __inline__ void DrawMasked(TileGrid *grid, s16 x, s32 y) {
    TileBlock *block;
    u32 width, height, size;
    s32 ix, iy;
    s32 i, j;

    iy = y;
    block = grid->m_Blocks;
    j = grid->m_nRows;
    height = 0;
    while (--j != -1) {
        i = grid->m_nColumns;
        ix = x;
        while (--i != -1) {
            height = block->m_Height;
            width = block->m_Width;
            size = width * height;
            func_00279158(block->m_Pixels, block->m_Pixels + size, ix, (s16) iy, width, height);
            ix += width;
            NEXT_BLOCK(block, size * 2 + (size >> 1));
        }
        iy += height;
    }
}

/* Draws frame `frame` of a tiled image at (x, y): block by block, row by row,
 * scaled by (scaleX, scaleY) when `scaled` is set; an image whose frame byte
 * is clear carries an opacity mask after each block's texels. */
void func_0027A7A0(void *table, s32 frame, s32 x, s32 y, f32 scaleX, f32 scaleY, s32 scaled) {
    TileGrid *grid;
    u8 opaque;

    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;                         /* gDPPipeSync */
        g->w1 = 0;
    }
    if (table != NULL) {
        func_0027AE44(table, frame, &grid, &opaque);
        if (grid != NULL) {
            if (opaque) {
                if (scaled) {
                    DrawScaled(grid, (s16) x, (s16) y, scaleX, scaleY);
                } else {
                    Draw(grid, x, y);
                }
            } else if (scaled) {
                DrawScaledMasked(grid, (s16) x, (s16) y, scaleX, scaleY);
            } else {
                DrawMasked(grid, x, y);
            }
            func_0020565C(0, table);
        }
    }
}
