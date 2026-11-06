/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_12.c, func_802B3A44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libaudio.h"

extern s32 __nextSampleTime(ALSynth *drvr, ALPlayer **client);
extern s32 _timeToSamples(ALSynth *, s32);
extern void _collectPVoices(ALSynth *);
Acmd *alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen)
{
    ALPlayer *client;
    ALFilter *output;
    ALSynth *drvr = &alGlobals->drvr;
    s16 tmp = 0;
    Acmd *cmdlEnd = cmdList;
    Acmd *cmdPtr;
    s32 nOut;
    s16 *lOutBuf = outBuf;
    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }
    for (drvr->paramSamples = __nextSampleTime(drvr, &client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = __nextSampleTime(drvr, &client)) {
        drvr->paramSamples &= ~0xf;
        client->samplesLeft += _timeToSamples(drvr, (*client->handler)(client));
    }
    drvr->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = ((drvr->maxOutSamples) < (outLen) ? (drvr->maxOutSamples) : (outLen));
        cmdPtr = cmdlEnd;
        { Acmd *_a = (Acmd *)(cmdPtr++); _a->words.w0 = (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); };
        output = drvr->outputFilter;
        (*output->setParam)(output, 6, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples, cmdPtr);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        drvr->curSamples += nOut;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);
    _collectPVoices(drvr);
    return cmdlEnd;
}
