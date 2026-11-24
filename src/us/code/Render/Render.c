/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80268CE0_de.c, func_80268CE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "gfx.h"
#include "globals/render.h"

void Render_SetCombineMode(u32 mode) {
 if (gCurrentCombineMode != mode) {
 gCurrentCombineMode=mode;
 switch(mode) {
 case 0: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 1: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 2: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 3: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 4: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 5: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(13) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 6: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 7: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 8: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 9: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 10: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 11: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 12: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(13) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 13: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 14: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 15: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 16: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 17: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 18: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 19: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 20: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 21: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 22: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 23: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 24: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 25: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 26: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 27: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 28: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 29: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 30: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); };
 case 31: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 32: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 33: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 34: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 35: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(13) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 36: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 37: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 38: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 39: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 40: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 case 41: { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); }; break;
 }
 }
}

/* Render_SetRenderMode, Render_DrawShadowQuad and Render_SetupMaterial, one source file: the
 * file's .rodata opens at 0x800A52B8 with Render_SetRenderMode's jump table, and
 * Render_SetupMaterial's pool (2^31 floats for float-to-unsigned and three jump
 * tables) follows at 0x800A5334, 4 past an 8-byte boundary, which it only
 * reaches as a continuation of that pool. Render_DrawShadowQuad sits between the two
 * in .text, so it is in the file too. Render_DrawShadowQuad and Render_SetupMaterial are the
 * Rage Wars imports. */
/* Render_DrawShadowQuad: from AbuN64/ragewars-decomp (Turok: Rage Wars), src/func_80268CE0_de.c, func_8026992C_de. */
/* Render_SetupMaterial: after AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/80268160_12.CPP, func_80269A80_de, with Turok 2's layouts. */
#include "common.h"
#include "gbi_t2.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "structs/light.h"
#include "protos/cache.h"
#include "structs/batch.h"

typedef struct TextureFrame {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 speed;
    /* 0x2 */ u8 shiftS;
    /* 0x3 */ u8 shiftT;
    /* 0x4 */ u8 lod;
    /* 0x5 */ u8 kind;
} TextureFrame;
typedef struct TextureInfo {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 palettes;
    /* 0x8 */ TextureFrame *frame;
} TextureInfo;
void Texture_InvalidateCache(void);

/* Switches the RDP render mode; returns 0 when it was already active. */
s32 Render_SetRenderMode(u32 mode) {
    if (gCurrentRenderMode == mode) {
        return 0;
    }
    gCurrentRenderMode = mode;
    switch (mode) {
    case 0:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0xC1849D8);
        } else {
            gDPSetRenderMode(gDList++, 0xC184A50);
        }
        break;
    case 1:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x1049D8);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104A50);
        }
        break;
    case 2:
        gDPSetRenderMode(gDList++, 0xC184A50);
        break;
    case 3:
        gDPSetRenderMode(gDList++, 0xC184B50);
        break;
    case 4:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104DD8);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104E50);
        }
        break;
    case 5:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x1041F8);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x1041F0);
        }
        break;
    case 6:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x113078);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x113078);
        }
        break;
    case 7:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112E10);
        break;
    case 8:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112478);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112478);
        }
        break;
    case 9:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112438);
        break;
    case 10:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112078);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112230);
        }
        break;
    case 11:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112038);
        break;
    case 12:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x5045D8);
        } else {
            gDPSetRenderMode(gDList++, 0x4045D0);
        }
        break;
    case 13:
        gDPSetRenderMode(gDList++, 0x504B50);
        break;
    case 14:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x5049D8);
        } else {
            gDPSetRenderMode(gDList++, 0x504A50);
        }
        break;
    case 15:
        gDPSetRenderMode(gDList++, 0x504E50);
        break;
    case 16:
        gDPSetRenderMode(gDList++, 0x504B53);
        break;
    case 17:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x553078);
        } else {
            gDPSetRenderMode(gDList++, 0x553078);
        }
        break;
    case 18:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x553048);
        } else {
            gDPSetRenderMode(gDList++, 0xF0A7008);
        }
        break;
    case 19:
        gDPSetRenderMode(gDList++, 0x504340);
        break;
    case 20:
        gDPSetRenderMode(gDList++, 0);
        break;
    case 21:
        gDPSetRenderMode(gDList++, 0x504240);
        break;
    case 22:
        gDPSetRenderMode(gDList++, 0xFA54040);
        break;
    case 23:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104B50);
        break;
    case 24:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104E50);
        break;
    case 25:
        gDPSetRenderMode(gDList++, 0xC184240);
        break;
    case 26:
        gDPSetRenderMode(gDList++, 0xC184340);
        break;
    case 27:
        gDPSetRenderMode(gDList++, 0xF0A4000);
        break;
    case 28:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x507048);
        } else {
            gDPSetRenderMode(gDList++, 0x507040);
        }
        break;
    case 29:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104F50);
        break;
    case 30:
        gDPSetRenderMode(gDList++, gFogRenderModeFogA | 0x104F50);
        break;
    }
    return 1;
}

/* Draws a 16x16 intensity texture (gShadowTexture) with `matrix` and the display
 * list for `pass`, white at `alpha`; then forces the next render and combine
 * modes to be set again. */
void Render_DrawShadowQuad(void *matrix, s32 pass, u32 alpha) {
    Gfx *list;

    /* gDPLoadTextureBlock(gShadowTexture, G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, ...) */
    gDPSetTextureImage(gDList++, G_IM_FMT_I, G_IM_SIZ_16b, 1, gShadowTexture);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDList++);
    gDPLoadBlockN(gDList++, G_TX_LOADTILE, 0, 0, 127, 1024);
    gDPPipeSync(gDList++);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_8b, 2, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(gDList++, G_TX_RENDERTILE, 0, 0, 15 << 2, 15 << 2);
    gDPSetPrimColor(gDList++, 0, 0, 255, 255, 255, alpha);
    gSPMatrixLoad(gDList++, matrix);
    list = sShadowQuadDL;
    gSPDisplayList(gDList++, ({ if (pass != 0) list = sShadowQuadDL2; list; }));
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
    Texture_InvalidateCache();
}

/* Scene colour mode: 0 none; otherwise an environment colour (and, for 6, a prim alpha). */

extern void *Texture_Request(s32 *texture);
extern void Texture_GetInfo(void *node, TextureInfo *info);
extern void Texture_Load(s32 *, void *, s32, s32, s32, s32, s32, s32, s32);

/* Sets up the RDP for drawing with `mat`: its texture frame `pass` (or the
 * animated one when negative), colours, combiner, render and geometry modes.
 * Returns 0 when the material is invisible or its texture is not loaded. */
s32 Render_SetupMaterial(Material *mat, s32 pass) {
    TextureInfo info;
    void *node;
    s32 tailEnum;
    s32 a;
    s32 b;
    u32 maskS;
    u32 maskT;
    u32 extra;
    u32 extra2;
    u32 mode;

    if ((mat->flags & 0x100) && mat->prim[3] == 0) {
        return 0;
    }
    tailEnum = -1;
    if (mat->texture != 0 && !(gEngineApp_m_Settings.cheatFlags & 0x400)) {
        node = Texture_Request(&mat->texture);
        if (node == 0) {
            return 0;
        }
        Texture_GetInfo(node, &info);
        if (pass < 0) {
            pass = gFrameCountGameplay / info.frame->speed;
        }
        a = pass % info.count;
        if (info.palettes != 0) {
            b = pass % info.palettes;
        } else {
            b = 0;
        }
        if (mat->flags & 0x10) {
            maskS = 1 << (info.frame->shiftS + 6);
            maskT = 1 << (info.frame->shiftT + 6);
        } else {
            maskS = mat->scaleS;
            maskT = mat->scaleT;
        }
        if (mat->flags & 0x20) {
            extra = G_TX_CLAMP;
            extra2 = G_TX_CLAMP;
        } else {
            extra = G_TX_WRAP;
            extra2 = G_TX_WRAP;
        }
        if (mat->flags & 0x800) {
            extra |= G_TX_MIRROR;
            extra2 |= G_TX_MIRROR;
        }
        Texture_Load(&mat->texture, node, a, b, maskS, maskT, extra, extra2, 0);
        if (mat->flags & 0x2000) {
            gDPSetTextureLOD(gDList++, G_TL_LOD);
        }
        gDPSetPrimColor(gDList++, info.frame->lod, 0, mat->prim[0], mat->prim[1], mat->prim[2], mat->prim[3]);
        if (mat->flags & 0x400) {
            gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2],
                           gBlendState == 0 ? mat->prim[3] : gBlendState_m_Color[3]);
            if (mat->flags & 2) {
                if (!(mat->flags & 0x1000)) {
                    Render_SetCombineMode(2);
                } else if (gBlendState == 0) {
                    Render_SetCombineMode(0);
                } else {
                    Render_SetCombineMode(1);
                }
            } else if (!(mat->flags & 0x1000)) {
                Render_SetCombineMode(4);
            } else {
                Render_SetCombineMode(3);
            }
            CCache__Unlock(0, node);
        } else if (mat->flags & 2) {
            if (mat->flags & 0x2000) {
                switch (info.frame->kind) {
                case 4:
                case 5:
                    gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    Render_SetCombineMode(0x23);
                    break;
                default:
                    Render_SetCombineMode(5);
                    break;
                }
            } else if (gBlendState == 0) {
                Render_SetCombineMode(6);
            } else {
                gDPSetEnvColor(gDList++, gBlendState_m_Color[0], gBlendState_m_Color[1], gBlendState_m_Color[2],
                               gBlendState_m_Color[3]);
                switch (gBlendState) {
                case 1:
                case 2:
                    Render_SetCombineMode(7);
                    break;
                case 3:
                    Render_SetCombineMode(8);
                    break;
                case 7:
                    Render_SetCombineMode(9);
                    break;
                case 4:
                case 5:
                    Render_SetCombineMode(10);
                    break;
                case 6:
                    gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gBlendState_m_Color[4]);
                    Render_SetCombineMode(0xB);
                    break;
                default:
                    break;
                }
            }
            CCache__Unlock(0, node);
        } else {
            if (mat->flags & 0x2000) {
                switch (info.frame->kind) {
                case 4:
                    gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    Render_SetCombineMode(0x25);
                    break;
                case 5:
                    gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    Render_SetCombineMode(0x24);
                    break;
                default:
                    Render_SetCombineMode(0xC);
                    break;
                }
            } else if (gBlendState == 0) {
                Render_SetCombineMode(0xD);
            } else {
                gDPSetEnvColor(gDList++, gBlendState_m_Color[0], gBlendState_m_Color[1], gBlendState_m_Color[2],
                               gBlendState_m_Color[3]);
                switch (gBlendState) {
                case 1:
                case 2:
                    Render_SetCombineMode(0xE);
                    break;
                case 3:
                    Render_SetCombineMode(0xF);
                    break;
                case 7:
                    Render_SetCombineMode(9);
                    break;
                case 4:
                case 5:
                    Render_SetCombineMode(0x10);
                    break;
                case 6:
                    gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gBlendState_m_Color[4]);
                    Render_SetCombineMode(0x11);
                    break;
                default:
                    break;
                }
            }
            CCache__Unlock(0, node);
        }
    } else {
        gDPPipeSync(gDList++);
        if (gPrimColorOverride != 0) {
            if (gPrimAlphaOverride != 0) {
                gDPSetPrimColor(gDList++, 0, 0, gPrimColorOverrideR, gPrimColorOverrideG, gPrimColorOverrideB, gPrimAlphaOverrideValue);
            } else {
                gDPSetPrimColor(gDList++, 0, 0, gPrimColorOverrideR, gPrimColorOverrideG, gPrimColorOverrideB, mat->prim[3]);
            }
        } else if (gPrimAlphaOverride != 0) {
            gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gPrimAlphaOverrideValue);
        } else {
            gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], mat->prim[3]);
        }
        gSPTexture(gDList++, 0, 0, 0, 0, 0);
        if (mat->flags & 2) {
            Render_SetCombineMode(0x12);
        } else if (gBlendState == 0) {
            Render_SetCombineMode(0x13);
        } else {
            gDPSetEnvColor(gDList++, gBlendState_m_Color[0], gBlendState_m_Color[1], gBlendState_m_Color[2],
                           gBlendState_m_Color[3]);
            switch (gBlendState) {
            case 1:
            case 2:
                Render_SetCombineMode(0x14);
                break;
            case 3:
                Render_SetCombineMode(0x15);
                break;
            case 7:
                Render_SetCombineMode(0x16);
                break;
            case 4:
            case 5:
                Render_SetCombineMode(0x17);
                break;
            case 6:
                gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gBlendState_m_Color[4]);
                Render_SetCombineMode(0x11);
                break;
            default:
                break;
            }
        }
    }
    if ((mat->flags & 0x100) || gBlendState == 1 || gBlendState == 4 || gBlendState == 6) {
        if (gBlendState == 0) {
            tailEnum = 3;
            if (!(mat->flags & 0x1000)) {
                tailEnum = 1;
                if (mat->flags & 0x200) {
                    tailEnum = 4;
                }
            }
        } else {
            tailEnum = 5;
            if (mat->flags & 0x1000) {
                tailEnum = 3;
            }
        }
    } else if (mat->flags & 0x80) {
        Render_SetRenderMode(6);
    } else {
        tailEnum = 7;
        if (!(mat->flags & 0x200)) {
            if (gRenderZModeInter != 0) {
                tailEnum = 9;
                if (gRenderImageRead != 0) {
                    tailEnum = 8;
                }
            } else {
                tailEnum = 0xB;
                if (gRenderImageRead != 0) {
                    tailEnum = 10;
                }
            }
        }
    }
    if (tailEnum != -1) {
        Render_SetRenderMode(tailEnum);
    }
    mode = 0x260404;
    if (mat->flags & 0x1000) {
        mode |= gFogGeometryMode;
    }
    gSPGeometryMode(gDList++, mode, 0);
    mode = 0;
    if (!(mat->flags & 0x1000)) {
        mode = gFogGeometryMode;
    }
    if (gBlendState == 7) {
        mode |= 0x20000;
    }
    if (!(mat->flags & 0x42)) {
        mode |= 0x20000;
    }
    mode |= 0x200004;
    if (!(mat->flags & 4)) {
        mode |= 0x400;
    }
    if (mat->flags & 0x10) {
        mode |= 0x60000;
    }
    gSPGeometryMode(gDList++, 0, mode);
    return 1;
}

extern void Texture_InvalidateCache();

void Render_BeginDeferredAlpha(void) {
    Gfx *cmd;
    Texture_InvalidateCache();
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = (GBI_ADDR(sDeferredAlphaTexture)); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(1024) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(124) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(124) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xD7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (11)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (8)) | (((unsigned int)(((unsigned int)(1))) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->w1 = ((((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (16)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (14) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 14))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xD9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(~(0)) & (0xFFFFFFFFU >> (32 - (24)))) << (0))); _gbi->w1 = ((unsigned int)(gFogGeometryMode | 0x00260404)); };
    Render_SetRenderMode(0x1D);
}

#include "protos/iset.h"
#include "structs/gbi.h"

/* A vertex as the morph frames store it. */

/* A morph frame: a vertex count and the vertices. */
typedef struct MorphFrame {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ Vtx_tn verts[1];
} MorphFrame;

/* Blends the morph frames around time t into the output frame: t wraps into [0, frames - 2), and
 * the two frames either side of it are mixed, positions in 16.16 and normals in 8.8 fixed point. */
void Model_BlendMorphFrames(void *resource, f32 t, void *unused) { /* Render_DrawModelMorph passes a third argument */
    MorphFrame *frames[2];
    s32 counts[2];
    Vtx_tn *verts[2];
    void *anim;
    MorphFrame *outFrame;
    Vtx_tn *out;
    s32 outCount;
    s32 *state;
    s32 numFrames;
    s32 i;
    s32 j;
    f32 frac;
    s32 n1;
    s32 n0;
    s32 p1;
    s32 p0;

    state = CISet__GetBlock(resource, 3);
    state[0] = gFrameCount;
    anim = CISet__GetBlock(resource, 0);
    numFrames = *(s32 *)anim;
    if (numFrames < 3) {
        return;
    }
    outFrame = CISet__GetBlock(anim, gEvenOdd);
    out = outFrame->verts;
    outCount = outFrame->count;
    while (t < 0.0f) {
        t += numFrames - 2;
    }
    while (t >= numFrames - 2) {
        t -= numFrames - 2;
    }
    frac = t - (s32)t;
    ((f32 *)state)[1] = t;
    n1 = frac * 256.0f;
    n0 = 0x100 - n1;
    p1 = frac * 65536.0f;
    p0 = 0x10000 - p1;
    for (i = 0; i < 2; i++) {
        frames[i] = CISet__GetBlock(anim, ((s32)t + i) % (numFrames - 2) + 2);
        counts[i] = frames[i]->count;
        verts[i] = frames[i]->verts;
    }
    if (counts[0] != outCount || counts[1] != outCount) {
        return;
    }
    for (j = 0; j < outCount; j++) {
        Vtx_tn *a = &verts[0][j];
        Vtx_tn *b = &verts[1][j];

        out[j].ob[0] = (a->ob[0] * p0 + b->ob[0] * p1) >> 16;
        out[j].ob[1] = (a->ob[1] * p0 + b->ob[1] * p1) >> 16;
        out[j].ob[2] = (a->ob[2] * p0 + b->ob[2] * p1) >> 16;
        out[j].n[0] = (a->n[0] * n0 + b->n[0] * n1) >> 8;
        out[j].n[1] = (a->n[1] * n0 + b->n[1] * n1) >> 8;
        out[j].n[2] = (a->n[2] * n0 + b->n[2] * n1) >> 8;
    }
}

#include "structs/table.h"
/* F3DEX2 triangle word: three vertex indices, each times 2 (libultra's __gsSP1Triangle_w1). */
#define TRI_WORD(a, b, c) (((((a) * 2) & 0xFF) << 16) | ((((b) * 2) & 0xFF) << 8) | (((c) * 2) & 0xFF))
/* Bytes of an offset table of n entries: the count, n offsets and the next free offset, 8-aligned. */
#define TABLE_SIZE(n) (((n) * 4 + 0xF) & ~7)

s32 ProPackDecompress(void ***handle, s32 arg1);
void Material_Unpack(void *dst, void *material);
void *func_0029E460(void *dst, const void *src, u32 n); /* memcpy */

/* Sets an offset table's entry count and the offset where the next entry starts. */
static __inline__ void TableSet(s32 *tbl, s32 count, s32 end) {
    OffsetTable *t = (OffsetTable *)tbl;
    t->count = count;
    t->offsets[count] = end;
}

/* Appends an entry to an offset table: bumps its count and records where the next entry starts. */
static __inline__ s32 TableAppend(s32 *tbl, s32 count, s32 end) {
    count++;
    TableSet(tbl, count, end);
    return count;
}

/* Builds the drawable form of a packed model resource. Its entries are offset tables (a count,
 * then the offset of each entry and of the end): the vertex frames, a block copied as is, the
 * levels of detail (each a table of parts: a material and a packed command stream) and, with
 * withExtra, three more blocks copied as is. The packed commands are expanded into a display list:
 * 0x80 one or two triangles (5-bit vertex indices), 0x20 a vertex load from the matching frame,
 * 0x40 a matrix load from segment 1, 0x60 a moveword. withTerminator adds a -1 terminator entry. */
void Model_Decompress(s32 unused, void ***handle, s32 withExtra, s32 withTerminator) {
    s32 vtxSize;
    s32 dataSize;
    s32 extra3Size;
    s32 extra4Size;
    s32 extra5Size;
    s32 frameSize;
    s32 lodCount;
    s32 lodOff;
    s32 vtxCounter;
    void *extra3;
    void *extra4;
    void *extra5;
    void *data;
    s32 *lods;
    s32 *lodTbl;
    s32 *parts;
    s32 *partTbl;
    u8 *base;
    Vtx_t *vtxBase;
    void **block;
    s32 *vtxTbl;
    s32 *frames;
    s32 *frameTbl;
    s32 vtxCount;
    s32 count;
    void *src;
    s32 size;
    s32 i;
    s32 frame;
    s32 j;
    s32 partCount;
    s32 n;
    u8 *p;
    u8 c;
    s32 *dst;
    s32 off;
    s32 partOff;
    s32 partSize;
    s32 *outPart;
    void *part;
    Gfx *start;
    Gfx *gfx;
    u32 w;
    s32 r;
    s32 g;
    s32 b;
    s32 first;
    s32 segment1;
    s32 cnt;
    s32 len;
    s32 mtx;
    s32 r2;
    s32 g2;
    s32 b2;

    if (ProPackDecompress(handle, 1) == 0) {
        return;
    }
    count = 6;
    if (withExtra == 0) {
        count = 3;
        if (withTerminator != 0) {
            count = 4;
        }
    }
    size = TABLE_SIZE(count);
    src = **handle;
    frames = CISet__GetBlockAndSize(src, 0, &vtxSize);
    vtxCount = *frames;
    size += vtxSize;
    data = CISet__GetBlockAndSize(src, 1, &dataSize);
    size += dataSize;
    lods = CISet__GetBlock(src, 2);
    lodCount = *lods;
    size += TABLE_SIZE(lodCount);
    for (i = 0; i < lodCount; i++) {
        parts = CISet__GetBlock(lods, i);
        partCount = *parts;
        size += TABLE_SIZE(partCount);
        for (j = 0; j < partCount; j++) {
            u8 op;

            /* Display list bytes: one command per packed command, plus the end and 8-alignment
             * (GFX_SIZE: 8 on the N64, 16 with the port's 64-bit Gfx). */
            part = CISet__GetBlock(parts, j);
            size += 0x30;
            p = CISet__GetBlock(part, 1);
            len = 0;
            op = *p;
            while (op != 0) {
                if (op & 0x80) {
                    p += 2;
                    len += GFX_SIZE;
                    if (*p & 0x80) {
                        p += 2;
                    }
                } else {
                    switch (op & 0x60) {
                    case 0x20:
                    case 0x40:
                        p += 2;
                        len += GFX_SIZE;
                        break;
                    case 0x60:
                        p += 5;
                        len += GFX_SIZE;
                        break;
                    }
                }
                op = *p;
            }
            size += (len + GFX_SIZE + 7) & ~7;
        }
    }
    if (withTerminator != 0) {
        size += 8;
    }
    if (withExtra != 0) {
        extra3 = CISet__GetBlockAndSize(src, 3, &extra3Size);
        size += extra3Size;
        extra4 = CISet__GetBlockAndSize(src, 4, &extra4Size);
        size += extra4Size;
        extra5 = CISet__GetBlockAndSize(src, 5, &extra5Size);
        size += extra5Size;
    }
    block = CCache__AllocateMemoryForDecompression(0, handle, size);
    if (block != NULL) {
        off = TABLE_SIZE(count);
        dst = *block;
        base = (u8 *)dst;
        n = TABLE_SIZE(vtxCount);
        vtxTbl = (s32 *)((u8 *)dst + off);
        TableSet(dst, 0, off);
        TableSet(vtxTbl, 0, n);
        frameTbl = vtxTbl;
        for (frame = 0; frame < vtxCount;) {
            CUSet__Transpose(CISet__GetBlockAndSize(frames, frame, &frameSize), base + off + n, 0);
            n += frameSize;
            frame = TableAppend(frameTbl, frame, n);
        }
        TableSet(dst, 1, off += n);
        func_0029E460(base + off, data, dataSize);
        lodOff = TABLE_SIZE(lodCount);
        TableSet(dst, 2, off += dataSize);
        lodTbl = (s32 *)(base + off);
        TableSet(lodTbl, 0, lodOff);
        segment1 = 0x01000000;
        for (i = 0; i < lodCount;) {
            parts = CISet__GetBlock(lods, i);
            vtxCounter = 0;
            partCount = *parts;
            partTbl = (s32 *)(base + off + lodOff);
            partOff = TABLE_SIZE(partCount);
            TableSet(partTbl, 0, partOff);
            vtxBase = (Vtx_t *)((u8 *)CISet__GetBlock(frameTbl, i) + 8);
            for (j = 0; j < partCount;) {
                part = CISet__GetBlock(parts, j);
                partSize = 0x10;
                outPart = (s32 *)(base + off + lodOff + partOff);
                TableSet(outPart, 0, partSize);
                Material_Unpack((u8 *)outPart + partSize, CISet__GetBlock(part, 0));
                partSize = 0x30;
                TableSet(outPart, 1, partSize);
                p = CISet__GetBlock(part, 1);
                start = (Gfx *)((u8 *)outPart + partSize);
                c = *p;
                gfx = start;
                while (c != 0) {
                    if (c & 0x80) {
                        w = *p++ << 8;
                        w |= *p++;
                        r = (w >> 10) & 0x1F;
                        g = (w >> 5) & 0x1F;
                        b = w & 0x1F;
                        if (*p & 0x80) {
                            w = *p++ << 8;
                            w |= *p++;
                            r2 = (w >> 10) & 0x1F;
                            g2 = (w >> 5) & 0x1F;
                            b2 = w & 0x1F;
                            GFX_W(gfx++, 0x06000000 | TRI_WORD(r, g, b), TRI_WORD(r2, g2, b2));     /* gSP2Triangles */
                        } else {
                            GFX_W(gfx++, 0x05000000 | TRI_WORD(r, g, b), 0);                   /* gSP1Triangle */
                        }
                    } else {
                        switch (c & 0x60) {
                        case 0x20:
                            w = *p++ << 8;
                            w |= *p++;
                            first = (w >> 5) & 0x1F;
                            cnt = w & 0x1F;
                            if (cnt == 0) {
                                cnt = 32;
                            }
                            GFX_W(gfx++, (cnt << 12) | (((first + cnt) << 1) | segment1),     /* gSPVertex */
                                  GBI_SEGADDR(GBI_ADDR(&vtxBase[vtxCounter]) - GBI_ADDR(vtxTbl) + 0x02000000)); /* segment 2 */
                            vtxCounter += cnt;
                            break;
                        case 0x40:
                            p++;
                            mtx = *p++;
                            GFX_W(gfx++, 0xDA380003, GBI_SEGADDR((mtx << 6) + segment1));                    /* gSPMatrix */
                            break;
                        case 0x60:
                            first = c & 0x1F;
                            p++;
                            w = *p++ << 24;
                            w |= *p++ << 16;
                            w |= *p++ << 8;
                            w |= *p++;
                            GFX_W(gfx++, (first << 1) | 0x02140000, w);                         /* moveword */
                            break;
                        }
                    }
                    c = *p;
                }
                len = (u8 *)gfx - (u8 *)start;
                partSize += (len + GFX_SIZE + 7) & ~7;
                partOff += partSize;
                GFX_W(gfx++, 0xDF000000, 0);                                                   /* gSPEndDisplayList */
                TableSet(outPart, 2, partSize);
                j = TableAppend(partTbl, j, partOff);
            }
            lodOff += partOff;
            i = TableAppend(lodTbl, i, lodOff);
        }
        TableSet(dst, 3, off += lodOff);
        if (withTerminator != 0) {
            s32 *e = (s32 *)(base + off);

            e[0] = -1;
            e[1] = 0;
            TableSet(dst, 4, off);
        }
        if (withExtra != 0) {
            func_0029E460(base + off, extra3, extra3Size);
            TableSet(dst, 4, off += extra3Size);
            func_0029E460(base + off, extra4, extra4Size);
            TableSet(dst, 5, off += extra4Size);
            func_0029E460(base + off, extra5, extra5Size);
            TableSet(dst, 6, off += extra5Size);
        }
    }
    CCache__DeallocAndReplaceCompressedData(0, handle, block);
}

#include "structs/frame.h"
#include "protos/game.h"

/* Draws one level of detail of a model resource when the frame's command buffer still has 3000
 * commands free: loads the matrix (or sets it as segment 1), sets segment 2 to the given texture
 * base or the model's own, loads an optional two-light block, then emits the display list of each
 * of the level's parts whose material Render_SetupMaterial accepts for the pass. */
void Render_DrawModel(void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod, s32 pass) {
    void *header;
    void **parts;
    void **lodParts;
    void *part;
    void *base;
    s32 count;
    s32 i;

    if (gDisplayListSize - ((uptr)gDList - (uptr)gEngineApp_m_pCurrentFrameData->m_pDisplayList) / sizeof(Gfx) < 3000) {
        return;
    }
    header = *resource;
    parts = CISet__GetBlock(header, 2);
    if (lod < 0 || lod >= *(s32 *)parts) {
        return;
    }
    CCache__LockForDisplayList(0, resource);
    if (segment != 0) {
        GFX_W(gDList++, 0xDB060004, GBI_ADDR(matrix));   /* gSPSegment(1, matrix) */
    } else {
        GFX_W(gDList++, 0xDA380003, GBI_ADDR(matrix));   /* gSPMatrix(matrix, G_MTX_LOAD) */
    }
    base = textures;
    if (base == 0) {
        base = CISet__GetBlock(header, 0);
    }
    GFX_W(gDList++, 0xDB060008, GBI_ADDR(base));     /* gSPSegment(2, base) */
    if (lights != 0) {
        GFX_W(gDList++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(lights));        /* gSPLight(lights, 2): ambient */
    }
    lodParts = CISet__GetBlock(parts, lod);
    count = *(s32 *)lodParts;
    for (i = 0; i < count; i++) {
        part = CISet__GetBlock(lodParts, i);
        if (Render_SetupMaterial(CISet__GetBlock(part, 0), pass) != 0) {
            void *list = CISet__GetBlock(part, 1);

            GFX_W(gDList++, 0xDE000000, GBI_ADDR(list));     /* gSPDisplayList */
        }
    }
}

#include "structs/model.h"
#include "globals/batch.h"


/* Returns the node keyed key in the tree under n, or the node a new key would hang from. */
static __inline__ SortNode *find_node(SortNode *n, u32 key) {
    SortNode *parent = NULL;

    while (n != NULL) {
        if (n->key == key) {
            return n;
        }
        parent = n;
        if (key < n->key) {
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return parent;
}

/* Flushes the batched model: files each part of its level of detail under its material key into
 * the opaque or the translucent (flags 0x700) sort tree, allocating a node when the key is new,
 * and appends one draw item per queued instance to the node's list; then empties the queue. */
void Render_FlushModelBatch(void) {
    void *header;
    s32 *parts;
    s32 count;
    s32 i;
    s32 j;
    void *part;
    Material *material;
    u32 key;
    SortNode *parent;
    SortNode *node;
    void *list;
    DrawItem *item;
    CModelInstance *inst;

    if (sBatchInstanceCount != 0) {
        header = *sBatchResource;
        for (j = 0; j < sBatchInstanceCount; j++) {
            inst = &sBatchInstances[j];
            if (inst->m_pTextures == NULL) {
                inst->m_pTextures = CISet__GetBlock(header, 0);
            }
        }
        parts = CISet__GetBlock(CISet__GetBlock(header, 2), sBatchLod);
        count = *parts;
        for (i = 0; i < count; i++) {
            part = CISet__GetBlock(parts, i);
            material = CISet__GetBlock(part, 0);
            /* byte 3 of the pass word: its low byte on the N64 (a symbol of its own at 0x800D8E13 before); on a
             * little-endian host the high byte, as the port read it before (kept: same output) */
            key = (material->id << 8) | ((u8 *)&sBatchPass)[3];
            if (material->flags & 0x700) {
                parent = find_node(sTranslucentSortRoot, key);
            } else {
                parent = find_node(sOpaqueSortRoot, key);
            }
            if (parent != NULL && parent->key == key) {
                node = parent;
            } else {
                if (sSortNodeCount == 100) {
                    break;
                }
                node = &sSortNodes[sSortNodeCount++];
                node->key = key;
                node->material = material;
                node->tail = NULL;
                node->head = NULL;
                if (material->flags & 0x700) {
                    if (parent != NULL) {
                        if (node->key < parent->key) {
                            parent->left = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent;
                            node->prev = parent->prev;
                            if (parent->prev != NULL) {
                                parent->prev->next = node;
                            }
                            parent->prev = node;
                            if (sTranslucentSortFirst == parent) {
                                sTranslucentSortFirst = node;
                            }
                        } else {
                            parent->right = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent->next;
                            if (parent->next != NULL) {
                                parent->next->prev = node;
                            }
                            node->prev = parent;
                            parent->next = node;
                        }
                    } else {
                        sTranslucentSortRoot = node;
                        sTranslucentSortFirst = node;
                        node->right = NULL;
                        node->left = NULL;
                        node->next = NULL;
                        node->prev = NULL;
                    }
                } else {
                    if (parent != NULL) {
                        if (node->key < parent->key) {
                            parent->left = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent;
                            node->prev = parent->prev;
                            if (parent->prev != NULL) {
                                parent->prev->next = node;
                            }
                            parent->prev = node;
                            if (sOpaqueSortFirst == parent) {
                                sOpaqueSortFirst = node;
                            }
                        } else {
                            parent->right = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent->next;
                            if (parent->next != NULL) {
                                parent->next->prev = node;
                            }
                            node->prev = parent;
                            parent->next = node;
                        }
                    } else {
                        sOpaqueSortRoot = node;
                        sOpaqueSortFirst = node;
                        node->right = NULL;
                        node->left = NULL;
                        node->next = NULL;
                        node->prev = NULL;
                    }
                }
            }
            list = CISet__GetBlock(part, 1);
            for (j = 0; j < sBatchInstanceCount; j++) {
                inst = &sBatchInstances[j];
                if (sDrawItemCount != 500) {
                    item = &sDrawItems[sDrawItemCount++];
                    item->list = list;
                    item->matrix = inst->m_pMatrix;
                    item->lights = inst->m_pLights;
                    item->textures = inst->m_pTextures;
                    item->scaleS = material->scaleS;
                    item->scaleT = material->scaleT;
                    item->segment = inst->m_Segment;
                    item->next = NULL;
                    if (node->tail != NULL) {
                        node->tail->next = item;
                    } else {
                        node->head = item;
                    }
                    node->tail = item;
                }
            }
        }
        sBatchInstanceCount = 0;
    }
}

/* Emits the batched draws: the opaque nodes in key order (those whose material is flagged 0x4000
 * set aside and drawn afterwards with their alpha as primitive colour, after Render_BeginDeferredAlpha), then
 * the translucent ones. Each item loads its lights, its matrix (or segment 1) and its textures as
 * segment 2, sets the texture scale when it changes, and calls the part's display list. */
void Render_DrawBatches(void) {
    SortNode *node;
    SortNode *deferred;
    SortNode *next;
    DrawItem *item;
    u16 scaleS;
    u16 scaleT;

    node = sOpaqueSortFirst;
    deferred = NULL;
    if (node != NULL) {
        do {
            item = node->head;
            if (item != NULL && Render_SetupMaterial(node->material, node->pass)) {
                scaleT = 0;
                scaleS = 0;
                do {
                    if (item->lights != NULL) {
                        GFX_W(gDList++, 0xDB020000, 0x18);                  /* gSPNumLights(1) */
                        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(item->lights + 8)); /* gSPLight(lights + 8, 1) */
                        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(item->lights));       /* gSPLight(lights, 2) */
                    }
                    if (item->segment != 0) {
                        GFX_W(gDList++, 0xDB060004, GBI_ADDR(item->matrix));       /* gSPSegment(1, matrix) */
                    } else {
                        GFX_W(gDList++, 0xDA380003, GBI_ADDR(item->matrix));       /* gSPMatrix(matrix, G_MTX_LOAD) */
                    }
                    GFX_W(gDList++, 0xDB060008, GBI_ADDR(item->textures));         /* gSPSegment(2, textures) */
                    if (item->scaleS != 0) {
                        if (scaleS != item->scaleS || scaleT != item->scaleT) {
                            if (node->material->flags & 0x2000) {
                                GFX_W(gDList++, 0xD7002802, (item->scaleS << 16) | item->scaleT); /* gSPTexture(s, t, 5, 0, on) */
                            } else {
                                GFX_W(gDList++, 0xD7000002, (item->scaleS << 16) | item->scaleT); /* gSPTexture(s, t, 0, 0, on) */
                            }
                        }
                        scaleS = item->scaleS;
                        scaleT = item->scaleT;
                    }
                    GFX_W(gDList++, 0xDE000000, GBI_ADDR(item->list));             /* gSPDisplayList */
                    item = item->next;
                } while (item != NULL);
                if (node->material->flags & 0x2000) {
                    GFX_W(gDList++, 0xE3000F00, 0);                         /* gDPSetTextureLOD(G_TL_TILE) */
                }
                if (node->material->flags & 0x4000) {
                    next = node->next;
                    node->next = deferred;
                    deferred = node;
                    node = next;
                } else {
                    node = node->next;
                }
            } else {
                node = node->next;
            }
        } while (node != NULL);
    }
    if (deferred != NULL) {
        node = deferred;
        Render_BeginDeferredAlpha();
        do {
            item = node->head;
            if (item != NULL) {
                GFX_W(gDList++, 0xFA000000, node->material->env[3]);         /* gDPSetPrimColor(0, 0, 0, 0, 0, alpha) */
                do {
                    if (item->lights != NULL) {
                        GFX_W(gDList++, 0xDB020000, 0x18);
                        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(item->lights + 8));
                        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(item->lights));
                    }
                    if (item->segment != 0) {
                        GFX_W(gDList++, 0xDB060004, GBI_ADDR(item->matrix));
                    } else {
                        GFX_W(gDList++, 0xDA380003, GBI_ADDR(item->matrix));
                    }
                    GFX_W(gDList++, 0xDB060008, GBI_ADDR(item->textures));
                    GFX_W(gDList++, 0xDE000000, GBI_ADDR(item->list));
                    item = item->next;
                } while (item != NULL);
            }
            node = node->next;
        } while (node != NULL);
    }
    node = sTranslucentSortFirst;
    if (node != NULL) {
        do {
            item = node->head;
            if (item != NULL && Render_SetupMaterial(node->material, node->pass)) {
                scaleT = 0;
                scaleS = 0;
                do {
                    if (item->lights != NULL) {
                        GFX_W(gDList++, 0xDB020000, 0x18);
                        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(item->lights + 8));
                        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(item->lights));
                    }
                    if (item->segment != 0) {
                        GFX_W(gDList++, 0xDB060004, GBI_ADDR(item->matrix));
                    } else {
                        GFX_W(gDList++, 0xDA380003, GBI_ADDR(item->matrix));
                    }
                    GFX_W(gDList++, 0xDB060008, GBI_ADDR(item->textures));
                    if (item->scaleS != 0) {
                        if (scaleS != item->scaleS || scaleT != item->scaleT) {
                            if (node->material->flags & 0x2000) {
                                GFX_W(gDList++, 0xD7002802, (item->scaleS << 16) | item->scaleT);
                            } else {
                                GFX_W(gDList++, 0xD7000002, (item->scaleS << 16) | item->scaleT);
                            }
                        }
                        scaleS = item->scaleS;
                        scaleT = item->scaleT;
                    }
                    GFX_W(gDList++, 0xDE000000, GBI_ADDR(item->list));
                    item = item->next;
                } while (item != NULL);
                if (node->material->flags & 0x2000) {
                    GFX_W(gDList++, 0xE3000F00, 0);
                }
            }
            node = node->next;
        } while (node != NULL);
    }
}

/* Draws one level of detail of a model resource flat-shaded in the primitive colour with the given
 * alpha, when the frame's command buffer still has 3000 commands free: loads the matrix (or sets it
 * as segment 1), loads an optional two-light block, sets segment 2 to the given texture base or the
 * model's own, sets up the combiner and render state, then emits every part's display list. The
 * cached render state gCurrentCombineMode/gCurrentRenderMode is invalidated before and after. */
void Render_DrawModelFlat(u8 alpha, void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod) {
    void *header;
    void **parts;
    void **lodParts;
    void *part;
    void *list;
    void *base;
    s32 count;
    s32 i;
    u32 geom;
    Gfx *sync;

    if (gDisplayListSize - ((uptr)gDList - (uptr)gEngineApp_m_pCurrentFrameData->m_pDisplayList) / sizeof(Gfx) < 3000) {
        return;
    }
    header = *resource;
    parts = CISet__GetBlock(header, 2);
    if (lod < 0 || lod >= *(s32 *)parts) {
        return;
    }
    CCache__LockForDisplayList(0, resource);
    if (segment != 0) {
        GFX_W(gDList++, 0xDB060004, GBI_ADDR(matrix));   /* gSPSegment(1, matrix) */
    } else {
        GFX_W(gDList++, 0xDA380003, GBI_ADDR(matrix));   /* gSPMatrix(matrix, G_MTX_LOAD) */
    }
    if (lights != 0) {
        GFX_W(gDList++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(lights));        /* gSPLight(lights, 2): ambient */
    }
    base = textures;
    if (base == 0) {
        base = CISet__GetBlock(header, 0);
    }
    GFX_W(gDList++, 0xDB060008, GBI_ADDR(base));     /* gSPSegment(2, base) */
    lodParts = CISet__GetBlock(parts, lod);
    sync = gDList++;
    gCurrentCombineMode = -1;
    gCurrentRenderMode = -1;
    geom = gFogGeometryMode | 0x20000;                    /* G_LIGHTING */
    count = *(s32 *)lodParts;
    GFX_W(sync, 0xE7000000, 0);                     /* gDPPipeSync */
    GFX_W(gDList++, 0xE3001001, 0);             /* gDPSetTextureLUT(G_TT_NONE) */
    GFX_W(gDList++, 0xD7FF0002, 0x80008000);    /* gSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF) */
    GFX_W(gDList++, 0xFA000000, alpha);         /* gDPSetPrimColor(0, 0, 0, 0, 0, alpha) */
    GFX_W(gDList++, 0xFCFFFFFF, 0xFFFDF6FB);    /* gDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE) */
    GFX_W(gDList++, 0xE200001C, gFogRenderModeShadeA | 0x104E50); /* gDPSetRenderMode */
    GFX_W(gDList++, 0xD9D8FBFB, 0);             /* gSPClearGeometryMode */
    geom |= 0x200004;                               /* G_SHADE | G_SHADING_SMOOTH */
    geom |= 0x400;                                  /* G_CULL_BACK */
    GFX_W(gDList++, 0xD9FFFFFF, geom); /* gSPSetGeometryMode */
    for (i = 0; i < count; i++) {
        part = CISet__GetBlock(lodParts, i);
        CISet__GetBlock(part, 0);
        list = CISet__GetBlock(part, 1);
        GFX_W(gDList++, 0xDE000000, GBI_ADDR(list));     /* gSPDisplayList */
    }
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
}

#include "globals/scene.h"

/* Draws one level of detail of a model resource with the shared material gScene_m_Geom, when the
 * frame's command buffer still has 3000 commands free and Render_SetupMaterial accepts the material for
 * the pass: sets the render mode, selects blend 2, loads the matrix (or sets it as segment 1),
 * loads an optional two-light block, registers the resource, sets segment 2 to the given texture
 * base or the model's own and emits each part's display list. */
void Render_DrawModelShared(void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod, s32 pass) {
    void *header;
    void *base;
    void **lodParts;
    void *part;
    void *list;
    s32 count;
    s32 i;

    if (gDisplayListSize - ((uptr)gDList - (uptr)gEngineApp_m_pCurrentFrameData->m_pDisplayList) / sizeof(Gfx) < 3000) {
        return;
    }
    if (Render_SetupMaterial(gScene_m_Geom, pass) == 0) {
        return;
    }
    GFX_W(gDList++, 0xE200001C, 0x0C184F50);    /* gDPSetRenderMode */
    Render_SetCombineMode(2);
    if (segment != 0) {
        GFX_W(gDList++, 0xDB060004, GBI_ADDR(matrix));   /* gSPSegment(1, matrix) */
    } else {
        GFX_W(gDList++, 0xDA380003, GBI_ADDR(matrix));   /* gSPMatrix(matrix, G_MTX_LOAD) */
    }
    if (lights != 0) {
        GFX_W(gDList++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(lights));        /* gSPLight(lights, 2): ambient */
    }
    CCache__LockForDisplayList(0, resource);
    header = *resource;
    base = textures;
    if (base == 0) {
        base = CISet__GetBlock(header, 0);
    }
    GFX_W(gDList++, 0xDB060008, GBI_ADDR(base));     /* gSPSegment(2, base) */
    lodParts = CISet__GetBlock(CISet__GetBlock(header, 2), lod);
    count = *(s32 *)lodParts;
    for (i = 0; i < count; i++) {
        part = CISet__GetBlock(lodParts, i);
        CISet__GetBlock(part, 0);
        list = CISet__GetBlock(part, 1);
        GFX_W(gDList++, 0xDE000000, GBI_ADDR(list));     /* gSPDisplayList */
    }
}

#include "m2c_macros.h"

void Model_DecompressCallback(sptr arg0, sptr arg1) {
    Model_Decompress(arg0, arg1, 0, 0);
}

void Model_DecompressCallbackTerminated(sptr arg0, sptr arg1) {
    Model_Decompress(arg0, arg1, 0, 1);
}

void Model_DecompressCallbackExtra(sptr arg0, sptr arg1) {
    Model_Decompress(arg0, arg1, 1, 0);
}

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ f32 value;
} Param;

void Model_AdvanceMorph(sptr *arg0, f32 arg1) { /* the handle holds a pointer: s32 truncated it on the host */
    f32 temp_f0;
    sptr temp_s0;
    Param *temp_v0;

    temp_s0 = *arg0;
    temp_v0 = CISet__GetBlock(temp_s0, 3);
    if (temp_v0->id != gFrameCount) {
        temp_f0 = temp_v0->value + arg1;
        temp_v0->value = temp_f0;
        Model_BlendMorphFrames(temp_s0, temp_f0, temp_v0);
    }
}
void Render_InvalidateModeCache(void);

void Render_SetupFog(void) {
    Render_InvalidateModeCache();
    if (gFogEnabled) {
        gFogGeometryMode = 0x10000;
        gFogRenderModeFogA = 0xC4000000;
        gFogRenderModeShadeA = 0xC8000000;
    } else {
        gFogGeometryMode = 0;
        gFogRenderModeFogA = 0x0C080000;
        gFogRenderModeShadeA = 0x0C080000;
    }
}

void Render_InvalidateModeCache(void) {
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
}

void Render_ResetBatches(void) {
    sBatchResource = NULL;
    sBatchInstanceCount = 0;
    sSortNodeCount = 0;
    sDrawItemCount = 0;
    sBatchResourceCount = 0;
    sOpaqueSortFirst = NULL;
    sOpaqueSortRoot = NULL;
    sTranslucentSortFirst = NULL;
    sTranslucentSortRoot = NULL;
}
void Render_FlushBatches(void);

void Render_FlushBatches(void) {
    sptr temp_s0;
    s32 var_s1;
    Render_FlushModelBatch();
    var_s1 = 0;
    Render_DrawBatches();
    if (sBatchResourceCount > 0) {
        do {
            temp_s0 = (sptr) sBatchResources[var_s1];
            CCache__LockForDisplayList(0, temp_s0);
            CCache__Unlock(0, temp_s0);
            var_s1 += 1;
        } while (var_s1 < sBatchResourceCount);
    }
}

/* Queues an instance of one level of detail of a model resource. A different model, level or
 * pass, or a full queue, first flushes the batch (Render_FlushModelBatch) and registers the model, unless
 * 256 models are already registered. */
void Render_QueueModelInstance(void **resource, void *matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass) {
    s32 *parts;
    CModelInstance *inst;

    parts = CISet__GetBlock(*resource, 2);
    if (lod < 0 || lod >= *parts) {
        return;
    }
    if (sBatchResource != resource || sBatchLod != lod || sBatchPass != pass || sBatchInstanceCount == 0x20) {
        Render_FlushModelBatch();
        if (sBatchResourceCount == 0x100) {
            return;
        }
        CCache__TouchAndLock(0, resource);
        sBatchResources[sBatchResourceCount++] = resource;
    }
    sBatchResource = resource;
    sBatchLod = lod;
    sBatchPass = pass;
    inst = &sBatchInstances[sBatchInstanceCount++];
    inst->m_pMatrix = matrix;
    inst->m_Segment = segment;
    inst->m_pLights = lights;
    inst->m_pTextures = textures;
}

#include "globals/strings.h"

/* Loads the model resource for key through its 0x18-byte header, then queues an instance of one
 * level of detail of it with the model's own textures, as a loaded matrix (see Render_QueueModelInstance), and
 * releases both. */
void Render_QueueModelByKey(sptr key, void *matrix, void *lights, s32 lod, s32 pass) {
    void **header;
    void **resource;
    s32 *parts;
    s32 size;
    sptr dataKey;
    CModelInstance *inst;
    s32 n;

    header = CCache__Request(0, key, 0x18, 0, 0, 0, sRenderCacheDesc_SimpObjectIndex, 1);
    if (header == 0) {
        return;
    }
    dataKey = CISet__GetROMAddressAndSize(*header, key, 1, &size);
    resource = CCache__Request(0, dataKey, size, 0, 0, Model_DecompressCallbackTerminated, sRenderCacheDesc_Geometry, 1);
    if (resource != 0) {
        parts = CISet__GetBlock(*resource, 2);
        if (lod >= 0 && lod < *parts) {
            if (sBatchResource != resource || sBatchLod != lod || sBatchPass != pass || sBatchInstanceCount == 0x20) {
                Render_FlushModelBatch();
                if (sBatchResourceCount == 0x100) {
                    goto done;
                }
                CCache__TouchAndLock(0, resource);
                sBatchResources[sBatchResourceCount] = resource;
                sBatchResourceCount += 1;
            }
            sBatchResource = resource;
            sBatchLod = lod;
            sBatchPass = pass;
            n = sBatchInstanceCount;
            inst = &sBatchInstances[n++];
            sBatchInstanceCount = n;
            inst->m_pMatrix = matrix;
            inst->m_Segment = 0;
            inst->m_pLights = lights;
            inst->m_pTextures = 0;
        }
    done:
        CCache__Unlock(0, resource);
    }
    CCache__Unlock(0, header);
}

void Render_QueueModelByResource(sptr res, sptr arg1, sptr arg2, s32 index, s32 arg4) {
    s32 size;
    sptr file;
    sptr handle;
    CModelInstance *e;
    s32 *count;
    s32 n;

    file = CCache__Request(0, res, 0x18, 0, 0, 0, sRenderCacheDesc_SimpObjectIndex, 0);
    if (file != 0) {
        /* the cache entries hold host pointers: read them pointer-wide (--high-memory) */
        handle = CCache__Request(0, CISet__GetROMAddressAndSize(*(s32 **) file, res, 1, &size), size, 0, 0,
                               Model_DecompressCallbackTerminated, sRenderCacheDesc_Geometry, 0);
        if (handle != 0) {
            count = CISet__GetBlock(*(void **) handle, 2);
            if (index >= 0 && index < *count) {
                if (sBatchResource != (void **) handle || sBatchLod != index || sBatchPass != arg4 || sBatchInstanceCount == 32) {
                    Render_FlushModelBatch();
                    if (sBatchResourceCount == 256) {
                        goto done;
                    }
                    CCache__TouchAndLock(0, handle);
                    sBatchResources[sBatchResourceCount] = (void **) handle;
                    sBatchResourceCount++;
                }
                n = sBatchInstanceCount;
                sBatchResource = (void **) handle;
                sBatchLod = index;
                sBatchPass = arg4;
                e = &sBatchInstances[n++];
                sBatchInstanceCount = n;
                e->m_pMatrix = (void *)arg1;
                e->m_Segment = 0;
                e->m_pLights = (void *)arg2;
                e->m_pTextures = 0;
            }
        done:
            CCache__Unlock(0, handle);
        }
        CCache__Unlock(0, file);
    }
}

void Render_DrawModelImmediate(sptr arg0, s32 renderMode, s32 **model, sptr matrix, s32 projected,
                   sptr lights, s32 *segment, s32 group, s32 arg8) {
    s32 *obj;
    s32 *list;
    s32 *entry;
    s32 *dl;
    s32 count;
    s32 i;
    s32 *seg;

    if ((u32) (gDisplayListSize - ((u32) ((u8 *) gDList - (u8 *) gEngineApp_m_pCurrentFrameData->m_pDisplayList) / GFX_SIZE)) < 3000) {
        return;
    }
    if (Render_SetupMaterial(arg0, arg8) == 0) {
        return;
    }
    if (projected) {
        GFX_CMD(0xDB060004, matrix);
    } else {
        GFX_CMD(0xDA380003, matrix);
    }
    if (lights) {
        GFX_CMD(0xDB020000, 0x18);
        GFX_CMD(0xDC08060A, lights + 8);
        GFX_CMD(0xDC08090A, lights);
    }
    if (renderMode != -1) {
        Render_SetRenderMode(renderMode);
    }
    CCache__LockForDisplayList(0, model);
    obj = *model;
    seg = segment;
    if (seg == NULL) {
        seg = CISet__GetBlock(obj, 0);
    }
    GFX_CMD(0xDB060008, GBI_ADDR(seg));
    list = CISet__GetBlock(CISet__GetBlock(obj, 2), group);
    count = *list;
    for (i = 0; i < count; i++) {
        entry = CISet__GetBlock(list, i);
        CISet__GetBlock(entry, 0);
        dl = CISet__GetBlock(entry, 1);
        GFX_CMD(0xDE000000, GBI_ADDR(dl));
    }
}

sptr Texture_RequestSync(void *);                          /* extern */

void Model_RequestTextures(sptr *arg0)
{
  s32 *temp_v0;
  s32 *temp_v0_2;
  s32 temp_s2;
  s32 temp_s4;
  sptr temp_v0_4;
  s32 var_s0;
  s32 var_s1;
  void *temp_v0_3;
  temp_v0 = CISet__GetBlock(*arg0, 2);
  temp_s4 = *temp_v0;
  var_s1 = 0;
  if (temp_s4 > 0)
  {
    do
    {
      temp_v0_2 = CISet__GetBlock(temp_v0, var_s1);
      temp_s2 = *temp_v0_2;
      var_s0 = 0;
      if (temp_s2 > 0)
      {
        do
        {
          temp_v0_3 = CISet__GetBlock(CISet__GetBlock(temp_v0_2, var_s0), 0);
          if ((*((s32 *) (((s8 *) temp_v0_3) + 8))) != 0)
          {
            temp_v0_4 = Texture_RequestSync(temp_v0_3 + 8);
            if (temp_v0_4 != 0)
            {
              CCache__Unlock(0, temp_v0_4);
 do { } while (0);
            }
          }
          var_s0 += 1;
        }
        while (var_s0 < temp_s2);
      }
      var_s1 += 1;
    }
    while (var_s1 < temp_s4);
  }
}

void Model_TouchTextures(sptr *arg0) {
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 temp_a1;
    s32 temp_s2;
    s32 temp_s4;
    s32 var_s0;
    s32 var_s1;

    temp_v0 = CISet__GetBlock(*arg0, 2);
    temp_s4 = *temp_v0;
    var_s1 = 0;
    if (temp_s4 > 0) {
        do {
            temp_v0_2 = CISet__GetBlock(temp_v0, var_s1);
            temp_s2 = *temp_v0_2;
            var_s0 = 0;
            if (temp_s2 > 0) {
                do {
                    temp_a1 = ((s32 *) CISet__GetBlock(CISet__GetBlock(temp_v0_2, var_s0), 0))[2];
                    if (temp_a1 != 0) {
                        CCache__Touch(0, temp_a1);
                    }
                    var_s0 += 1;
                } while (var_s0 < temp_s2);
            }
            var_s1 += 1;
        } while (var_s1 < temp_s4);
    }
}

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 count;
} Handle;

void func_0020BFB0(Handle *arg0) {
    arg0->id = -1;
    arg0->count = 0;
}

void Render_ClearSortTrees(void) {
    sOpaqueSortFirst = NULL;
    sOpaqueSortRoot = NULL;
    sTranslucentSortFirst = NULL;
    sTranslucentSortRoot = NULL;
}
void *Render_SortTreeFind(Node8026E4F8 **arg0, u32 arg1);

void *Render_SortTreeFind(Node8026E4F8 **arg0, u32 arg1)
{
  u32 temp_v1;
  Node8026E4F8 *var_a0;
  Node8026E4F8 *var_v0;
  var_v0 = *arg0;
  var_a0 = 0;
  if (var_v0 != 0)
  {
    loop_1:
    temp_v1 = var_v0->key;
    if (var_v0->key)
    {
      var_a0 = var_v0;
    }
    else
    {
      var_a0 = var_v0;
    }
    if (temp_v1 != arg1)
    {
      if (arg1 < temp_v1)
      {
        var_v0 = var_a0->left;
      }
      else
      {
        var_v0 = var_a0->right;
      }
      if (var_v0 == 0)
      {
        goto block_6;
      }
      goto loop_1;
    }
  }
  else
  {
    block_6:
    var_v0 = var_a0;
  }
  return var_v0;
}

void Render_SortTreeInsert(TreeNode **root, TreeNode **empty_root, TreeNode *node, TreeNode *current) {
    if (current != 0) {
        if (node->key < current->key) {
            current->link18 = node;
            node->link1c = 0;
            node->link18 = 0;
            node->link14 = current;
            node->link10 = current->link10;
            if (current->link10 != 0)
                current->link10->link14 = node;
            current->link10 = node;
            if (*root == current)
                *root = node;
        } else {
            current->link1c = node;
            node->link1c = 0;
            node->link18 = 0;
            node->link14 = current->link14;
            if (current->link14 != 0)
                current->link14->link10 = node;
            node->link10 = current;
            current->link14 = node;
        }
    } else {
        *empty_root = node;
        *root = node;
        node->link1c = 0;
        node->link18 = 0;
        node->link14 = 0;
        node->link10 = 0;
    }
}
