#ifndef GLOBALS_AUTOMAP_H
#define GLOBALS_AUTOMAP_H

/* The automap (CAutomap, gEngineApp_m_Game_m_DebugLines): its matrices and
 * its double-buffered line vertices, one colour per buffer. */

#include "structs/gbi.h"

extern Mtx sAutomapWorldMtx[2];
extern Mtx sAutomapScreenMtx[2];
extern Mtx sAutomapOrthoMtx;
extern u16 sAutomapLineIndices[];       /* line vertex indices */
extern Vtx_t sAutomapLineVtx[2][0x200]; /* green */
extern Vtx_t sAutomapMagentaVtx[2][8];
extern Vtx_t sAutomapRedVtx[2][0xC0];
extern Vtx_t sAutomapYellowVtx[2][3];

#endif
