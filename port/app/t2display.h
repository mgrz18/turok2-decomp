// t2display.h - the player's display settings (round 18 g4, docs/port/MACOS.md section 20):
// the renderer the window uses and the resolution the game frame is drawn at.
#pragma once

#include <memory>

namespace Ship {
class Config;
}
namespace Fast {
class Fast3dWindow;
}

// The CVars (turok2.cfg.json, section "CVars"):
//   gT2Renderer     0 = OpenGL (the default: the renderer every check of the port used),
//                   1 = Metal (macOS, opt-in). Applied to Window.Backend.Id at each launch.
//   gT2RenderScale  the game frame's resolution as a multiple of the window's PIXELS (default 1:
//                   a retina window draws at its full pixel resolution; 0.5 = half, 2 = 2x
//                   supersampled). Clamped to 0.25..4.
void T2Display_RegisterSettings(bool save);

// Sets Window.Backend.Id from gT2Renderer, before Fast3dWindow::Init (which reads it).
void T2Display_ChooseRenderer(const std::shared_ptr<Ship::Config>& config);

// gT2RenderScale, or `override` when > 0 (--render-scale), clamped.
float T2Display_RenderScale(float override);

// Per drawn frame with a window: the interpreter's resolution multiplier = the window's pixel
// density (SDL_GetWindowPixelDensity: 2 on a retina display, 1 elsewhere) x the render scale.
// LUS sizes the game frame from the ImGui content region, which is in points on macOS.
void T2Display_UpdateWindow(const std::shared_ptr<Fast::Fast3dWindow>& window, float overrideScale);
