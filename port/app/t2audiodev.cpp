// t2audiodev.cpp - the sound follows the default output device (round 19 h4, t2audiodev.h,
// docs/port/MACOS.md section 21).
#include "t2audiodev.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <string>
#include <vector>

#include "libultraship/bridge/audiobridge.h"
#include "ship/audio/Audio.h"
#include "ship/config/Config.h"

#ifdef __APPLE__
#include <CoreAudio/CoreAudio.h>
#endif

namespace {
std::weak_ptr<Ship::Audio> sAudio;
std::shared_ptr<Ship::Config> sConfig;
std::atomic<int> sPending{ 0 }; // changes the listener saw and the game thread has not handled
std::atomic<int> sChanges{ 0 };
int sReopened = 0;
bool sInstalled = false;

#ifdef __APPLE__
const AudioObjectPropertyAddress kDefaultOutput = { kAudioHardwarePropertyDefaultOutputDevice,
                                                    kAudioObjectPropertyScopeGlobal,
                                                    kAudioObjectPropertyElementMain };

AudioObjectID DefaultOutput() {
    AudioObjectID id = kAudioObjectUnknown;
    UInt32 size = sizeof(id);
    if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &kDefaultOutput, 0, nullptr, &size, &id) != noErr) {
        return kAudioObjectUnknown;
    }
    return id;
}

std::string DeviceName(AudioObjectID id) {
    if (id == kAudioObjectUnknown) {
        return "none";
    }
    const AudioObjectPropertyAddress addr = { kAudioObjectPropertyName, kAudioObjectPropertyScopeGlobal,
                                              kAudioObjectPropertyElementMain };
    CFStringRef name = nullptr;
    UInt32 size = sizeof(name);
    if (AudioObjectGetPropertyData(id, &addr, 0, nullptr, &size, &name) != noErr || name == nullptr) {
        return "?";
    }
    char buf[256] = {};
    if (!CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8)) {
        buf[0] = '?';
    }
    CFRelease(name);
    return buf;
}

AudioObjectID sDevice = kAudioObjectUnknown; // the device the player was opened on

// CoreAudio calls it on one of its own threads: count only; the game thread re-creates the player
OSStatus OnDefaultOutputChanged(AudioObjectID, UInt32, const AudioObjectPropertyAddress*, void*) {
    sPending.fetch_add(1);
    sChanges.fetch_add(1);
    return noErr;
}
#endif
} // namespace

bool T2AudioDev_Install(const std::shared_ptr<Ship::Audio>& audio, const std::shared_ptr<Ship::Config>& config) {
#ifdef __APPLE__
    if (audio == nullptr || audio->GetCurrentAudioBackend() != Ship::AudioBackend::COREAUDIO || sInstalled) {
        return false;
    }
    if (AudioObjectAddPropertyListener(kAudioObjectSystemObject, &kDefaultOutput, OnDefaultOutputChanged, nullptr) !=
        noErr) {
        printf("audio: cannot watch the default output device: the sound stays on the current one\n");
        return false;
    }
    sAudio = audio;
    sConfig = config;
    sInstalled = true;
    sDevice = DefaultOutput();
    printf("audio: playing on \"%s\"; following changes of the default output device\n", DeviceName(sDevice).c_str());
    return true;
#else
    (void)audio;
    (void)config;
    return false;
#endif
}

void T2AudioDev_Uninstall() {
#ifdef __APPLE__
    if (sInstalled) {
        AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &kDefaultOutput, OnDefaultOutputChanged, nullptr);
        sInstalled = false;
    }
#endif
    sAudio.reset();
    sConfig.reset();
}

bool T2AudioDev_Poll() {
#ifdef __APPLE__
    if (!sInstalled || sPending.load(std::memory_order_relaxed) == 0) {
        return false;
    }
    sPending.store(0);
    auto audio = sAudio.lock();
    if (audio == nullptr) {
        return false;
    }
    // Audio::SetCurrentAudioBackend re-creates the player (a new AUHAL unit on the default device
    // of now; the old one stops and closes when its last reference goes, here) and writes the
    // backend into the config: keep what the player's config said (main.cpp keeps it too).
    auto config = sConfig;
    const std::string saved = config ? config->GetString("Window.AudioBackend", "") : "";
    const AudioObjectID from = sDevice;
    audio->SetCurrentAudioBackend(Ship::AudioBackend::COREAUDIO);
    if (config && config->GetString("Window.AudioBackend", "") != saved) {
        config->SetString("Window.AudioBackend", saved);
        config->Save();
    }
    sDevice = DefaultOutput();
    sReopened++;
    auto player = audio->GetAudioPlayer();
    const bool open = player != nullptr && player->IsInitialized() &&
                      audio->GetCurrentAudioBackend() == Ship::AudioBackend::COREAUDIO;
    if (open) {
        // start the new device with the latency the old one had (silence), so --pace's clock,
        // which follows the device's buffer (game.cpp), does not see an underrun
        static std::vector<uint8_t> zeros;
        const int frames = std::max(AudioPlayerGetDesiredBuffered(), 0);
        zeros.assign((size_t)frames * 4, 0);
        if (frames > 0) {
            AudioPlayerPlayFrame(zeros.data(), zeros.size());
        }
    }
    printf("audio: the default output changed (\"%s\" -> \"%s\"): sound player re-created, %s\n",
           DeviceName(from).c_str(), DeviceName(sDevice).c_str(),
           open ? "open" : "NOT open (no output device): silent until the next change");
    fflush(stdout);
    return true;
#else
    return false;
#endif
}

void T2AudioDev_SimulateChange() {
#ifdef __APPLE__
    // what CoreAudio does when the default output changes: the same callback, the same address
    if (sInstalled) {
        OnDefaultOutputChanged(kAudioObjectSystemObject, 1, &kDefaultOutput, nullptr);
    } else {
        printf("audio: --audio-device-change-test: no CoreAudio player is watched (no device open)\n");
    }
#endif
}

int T2AudioDev_Reopened() {
    return sReopened;
}

int T2AudioDev_Changes() {
    return sChanges.load();
}
