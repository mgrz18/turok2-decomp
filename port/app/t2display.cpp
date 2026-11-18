// t2display.cpp - the renderer and the game frame's resolution for a player's window
// (round 18 g4, t2display.h, docs/port/MACOS.md section 20).
#include "t2display.h"

#include <algorithm>
#include <cstdio>

#include <SDL3/SDL_video.h>

#include "fast/Fast3dWindow.h"
#include <libultraship/libultraship.h>
#include "ship/config/Config.h"

namespace {
const char* kRenderer = "gT2Renderer";
const char* kRenderScale = "gT2RenderScale";
} // namespace

void T2Display_RegisterSettings(bool save) {
    CVarRegisterInteger(kRenderer, 0);
    CVarRegisterFloat(kRenderScale, 1.0f);
    // Round 19 (h4): the game frame stays 4:3 in a window of any shape (pillarbox / letterbox,
    // black bars). Without it LUS sizes the game frame to the whole window: the game clears and
    // letterboxes only its own 4:3 part (the fill rectangles and the scissor are centred), so in a
    // 16:9 or 16:10 window (a resized window, fullscreen on a MacBook) the bars at the sides showed
    // stale frames and 3D drawn past the game's edges (seen headless with T2_OFFSCREEN_SIZE=1280x720,
    // MACOS.md 21). LUS's "advanced resolution" with a 4:3 aspect keeps the frame's height at the
    // window's pixels and centres it. A player who wants the old behaviour sets
    // gAdvancedResolution.Enabled = 0 in turok2.cfg.json.
    CVarRegisterInteger("gAdvancedResolution.Enabled", 1);
    CVarRegisterFloat("gAdvancedResolution.AspectRatioX", 4.0f);
    CVarRegisterFloat("gAdvancedResolution.AspectRatioY", 3.0f);
    CVarRegisterInteger("gAdvancedResolution.VerticalResolutionToggle", 0);
    CVarRegisterInteger("gAdvancedResolution.PixelPerfectMode", 0);
    CVarRegisterInteger("gAdvancedResolution.IgnoreAspectCorrection", 0);
    if (save) {
        CVarSave();
    }
}

void T2Display_ChooseRenderer(const std::shared_ptr<Ship::Config>& config) {
    // LUS lists Metal first on macOS, so a config without Window.Backend.Id (a first launch) got
    // Metal and saved it. The port's checks all ran on the OpenGL renderer (MACOS.md 19.3, 20):
    // OpenGL unless the player asks for Metal.
    int want = Fast::FAST3D_SDL_OPENGL;
#ifdef __APPLE__
    if (CVarGetInteger(kRenderer, 0) == 1) {
        want = Fast::FAST3D_SDL_METAL;
    }
#endif
    if (config->GetInt("Window.Backend.Id", -1) != want) {
        config->SetInt("Window.Backend.Id", want);
        config->SetString("Window.Backend.Name", want == Fast::FAST3D_SDL_OPENGL ? "OpenGL" : "Metal");
        config->Save();
    }
    printf("window: renderer %s (gT2Renderer %d: 0 OpenGL, 1 Metal)\n",
           want == Fast::FAST3D_SDL_OPENGL ? "OpenGL" : "Metal", CVarGetInteger(kRenderer, 0));
}

float T2Display_RenderScale(float override) {
    const float s = override > 0.0f ? override : CVarGetFloat(kRenderScale, 1.0f);
    return std::clamp(s > 0.0f ? s : 1.0f, 0.25f, 4.0f);
}

void T2Display_UpdateWindow(const std::shared_ptr<Fast::Fast3dWindow>& window, float overrideScale) {
    static SDL_Window* sdlWindow = nullptr;
    static float applied = -1.0f;
    if (sdlWindow == nullptr) {
        int n = 0;
        SDL_Window** windows = SDL_GetWindows(&n);
        if (windows != nullptr && n > 0) {
            sdlWindow = windows[0];
            // round 19 (h4): LUS sets no minimum size; a window dragged down to a few points made a
            // game frame of a few pixels (ApplyResolutionChanges clamps to 320x240 anyway)
            SDL_SetWindowMinimumSize(sdlWindow, 320, 240);
        }
        SDL_free(windows);
    }
    float density = sdlWindow != nullptr ? SDL_GetWindowPixelDensity(sdlWindow) : 1.0f;
    if (!(density > 0.0f)) {
        density = 1.0f;
    }
    const float mul = density * T2Display_RenderScale(overrideScale);
    if (mul != applied) {
        window->SetResolutionMultiplier(mul);
        printf("window: pixel density %.2f, render scale %.2f: the game frame at %.2f x the window's points\n",
               density, T2Display_RenderScale(overrideScale), mul);
        applied = mul;
    }
}
