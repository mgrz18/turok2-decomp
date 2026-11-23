#include "common.h"
#include "structs/sound.h"

void SndSeq_FadeOut(SndSeq *);                   /* extern */

void CAudioWorld__FadeOutMusic(CAudioWorld *arg0) {
    SndSeq_FadeOut(&arg0->seq);
}
