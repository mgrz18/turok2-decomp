#include "common.h"
#include "structs/sound.h"
#include "protos/audio_world.h"

void CAudioWorld__StopSpeech(CAudioWorld *m) {
    SpeechPlayer_Stop(&m->fx);
}
