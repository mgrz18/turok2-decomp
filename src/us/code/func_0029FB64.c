/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_10.c, func_802BEAA4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void alCopy(unsigned char *source, unsigned char *destination, int count);
extern void alAdpcmPull(void);
extern Acmd *alRaw16Pull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern Acmd *alAdecPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
s32 alLoadParam(void *filter, s32 paramID, ALWaveTable *param)
{
    ALLoadFilter *a = (ALLoadFilter *)filter;
    ALFilter *f = (ALFilter *)filter;
    switch (paramID) {
        case 5:
            a->table = param;
            a->memin = (s32)a->table->base;
            a->sample = 0;
            switch (a->table->type) {
                case 0:
                    f->handler = (void *) alAdpcmPull;
                    a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order *
                        a->table->waveInfo.adpcmWave.book->npredictors * 8;
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                        a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                        ((void (*)(void *, void *, s32))alCopy)(a->table->waveInfo.adpcmWave.loop->state,
                                      a->lstate, sizeof(ADPCM_STATE));
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 1:
                    f->handler = alRaw16Pull;
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.start = a->table->waveInfo.rawWave.loop->start;
                        a->loop.end = a->table->waveInfo.rawWave.loop->end;
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 2:
                    f->handler = alAdecPull;
                    a->loop.start = a->loop.end = a->loop.count = 0;
                    break;
                default:
                    break;
            }
            break;
        case 4:
            a->lastsam = 0;
            a->first = 1;
            a->sample = 0;
            if (a->table) {
                a->memin = (s32)a->table->base;
                if (a->table->type == 0) {
                    if (a->table->waveInfo.adpcmWave.loop)
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                } else if (a->table->type == 1) {
                    if (a->table->waveInfo.rawWave.loop)
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                }
            }
            break;
        default:
            break;
    }
}
