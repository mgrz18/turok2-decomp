/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_9.c, func_802B51B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 func_00296118(void *filter, s32 paramID, void *param)
{
    ALFilter_s *f = (ALFilter_s *)filter;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    switch (paramID) {
      case (3):
          if (e->ctrlTail) {
              e->ctrlTail->next = (ALParam_s *)param;
          } else {
              e->ctrlList = (ALParam_s *)param;
          }
          e->ctrlTail = (ALParam_s *)param;
          break;
      case (4):
          e->first = 1;
          e->motion = 0;
          e->volume = 1;
          if (f->source)
              (*f->source->setParam)(f->source, 4, param);
          break;
      case (9):
          e->motion = 1;
          if (f->source)
              (*f->source->setParam)(f->source, 9, param);
          break;
      case (1):
          f->source = (ALFilter_s *)param;
          break;
      default:
          if (f->source)
              (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}
