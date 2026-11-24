/* CMtxF, the 4x4 float matrix (structs/vector.h): prototypes from the definitions. */

#ifndef PROTOS_MTXF_H
#define PROTOS_MTXF_H

#include "structs/vector.h"

void CMtxF__SetRotateY(CMtxF mf, f32 angle);
void CMtxF__TransformPoint(CMtxF mf, CVector3 *in, CVector3 *out);

#endif
