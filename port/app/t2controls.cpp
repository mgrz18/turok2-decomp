// t2controls.cpp - the port's controls for a human player (round 14 c4); see t2controls.h.
#include "t2controls.h"

#include <libultraship/libultraship.h>

#include <SDL3/SDL.h>
#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

#include "libultraship/controller/controldeck/ControlDeck.h"
#include "libultraship/controller/controldevice/controller/mapping/ControllerDefaultMappings.h"
#include "ship/config/ConsoleVariable.h"
#include "ship/window/Window.h"
#include "ship/window/gui/Gui.h"

namespace {

using Ship::KbScancode;

std::shared_ptr<Ship::Window> sWindow;

// ---- the headless window (--pad-test) ----
// Every method a no-op, except the mouse, which the test script sets.
struct MouseFeed {
    int dx = 0, dy = 0;
    bool left = false, right = false, captured = false;
} sMouseFeed;

class HeadlessWindow : public Ship::Window {
  public:
    explicit HeadlessWindow(std::shared_ptr<Ship::ConsoleVariable>) : Ship::Window(std::shared_ptr<Ship::Config>()) {
        MarkInitialized(); // nothing to open; the ControlDeck checks it
    }
    void Close() override {
    }
    void RunGuiOnly() override {
    }
    void StartFrame() override {
    }
    void EndFrame() override {
    }
    bool IsFrameReady() override {
        return true;
    }
    void HandleEvents() override {
    }
    void SetCursorVisibility(bool) override {
    }
    uint32_t GetWidth() override {
        return 640;
    }
    uint32_t GetHeight() override {
        return 480;
    }
    float GetAspectRatio() override {
        return 4.0f / 3.0f;
    }
    int32_t GetPosX() override {
        return 0;
    }
    int32_t GetPosY() override {
        return 0;
    }
    void SetMousePos(Ship::Coords) override {
    }
    Ship::Coords GetMousePos() override {
        return { 0, 0 };
    }
    Ship::Coords GetMouseDelta() override {
        Ship::Coords c = { sMouseFeed.dx, sMouseFeed.dy };
        return c;
    }
    Ship::CoordsF GetMouseWheel() override {
        return { 0.0f, 0.0f };
    }
    bool GetMouseState(Ship::MouseBtn btn) override {
        return btn == Ship::LUS_MOUSE_BTN_LEFT ? sMouseFeed.left : btn == Ship::LUS_MOUSE_BTN_RIGHT && sMouseFeed.right;
    }
    void SetMouseCapture(bool capture) override {
        sMouseFeed.captured = capture;
    }
    bool IsMouseCaptured() override {
        return sMouseFeed.captured;
    }
    uint32_t GetCurrentRefreshRate() override {
        return 60;
    }
    bool SupportsWindowedFullscreen() override {
        return false;
    }
    bool CanDisableVerticalSync() override {
        return false;
    }
    void SetResolutionMultiplier(float) override {
    }
    void SetMsaaLevel(uint32_t) override {
    }
    void SetFullscreen(bool) override {
    }
    bool IsFullscreen() override {
        return false;
    }
    bool IsRunning() override {
        return true;
    }
    const char* GetKeyName(int32_t) override {
        return "?";
    }
    uintptr_t GetGfxFrameBuffer() override {
        return 0;
    }
    void SetCurrentDimensions(uint32_t, uint32_t) override {
    }
    void SetCurrentDimensions(uint32_t, uint32_t, int32_t, int32_t) override {
    }
    void SetCurrentDimensions(bool, uint32_t, uint32_t) override {
    }
    void SetCurrentDimensions(bool, uint32_t, uint32_t, int32_t, int32_t) override {
    }
    Ship::WindowRect GetPrimaryMonitorRect() override {
        return { 0, 0, 640, 480 };
    }
};

// ---- --pad-test ----
struct PadEvent {
    int frame;
    int kind; // 0 key, 1 gamepad button, 2 gamepad axis, 3 mouse move, 4 mouse button, 5 capture, 6 plug, 7 unplug
    int code;
    int value;
};
std::vector<PadEvent> sPadEvents;
bool sPadTest = false;
SDL_JoystickID sVirtualId = 0;
SDL_Joystick* sVirtual = nullptr;

int ParseCode(int kind, const std::string& s) {
    static const struct {
        const char* name;
        int code;
    } kKeys[] = { { "ESC", KbScancode::LUS_KB_ESCAPE },        { "TAB", KbScancode::LUS_KB_TAB },
                  { "Q", KbScancode::LUS_KB_Q },               { "W", KbScancode::LUS_KB_W },
                  { "E", KbScancode::LUS_KB_E },               { "T", KbScancode::LUS_KB_T },
                  { "ENTER", KbScancode::LUS_KB_ENTER },       { "CTRL", KbScancode::LUS_KB_CONTROL },
                  { "A", KbScancode::LUS_KB_A },               { "S", KbScancode::LUS_KB_S },
                  { "D", KbScancode::LUS_KB_D },               { "F", KbScancode::LUS_KB_F },
                  { "G", KbScancode::LUS_KB_G },               { "H", KbScancode::LUS_KB_H },
                  { "SPACE", KbScancode::LUS_KB_SPACE },       { "UP", KbScancode::LUS_KB_ARROWKEY_UP },
                  { "DOWN", KbScancode::LUS_KB_ARROWKEY_DOWN }, { "LEFT", KbScancode::LUS_KB_ARROWKEY_LEFT },
                  { "RIGHT", KbScancode::LUS_KB_ARROWKEY_RIGHT } },
      kButtons[] = { { "SOUTH", SDL_GAMEPAD_BUTTON_SOUTH },     { "EAST", SDL_GAMEPAD_BUTTON_EAST },
                     { "WEST", SDL_GAMEPAD_BUTTON_WEST },       { "NORTH", SDL_GAMEPAD_BUTTON_NORTH },
                     { "BACK", SDL_GAMEPAD_BUTTON_BACK },       { "START", SDL_GAMEPAD_BUTTON_START },
                     { "LB", SDL_GAMEPAD_BUTTON_LEFT_SHOULDER }, { "RB", SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER },
                     { "DUP", SDL_GAMEPAD_BUTTON_DPAD_UP },     { "DDOWN", SDL_GAMEPAD_BUTTON_DPAD_DOWN },
                     { "DLEFT", SDL_GAMEPAD_BUTTON_DPAD_LEFT }, { "DRIGHT", SDL_GAMEPAD_BUTTON_DPAD_RIGHT } },
      kAxes[] = { { "LX", SDL_GAMEPAD_AXIS_LEFTX },  { "LY", SDL_GAMEPAD_AXIS_LEFTY },
                  { "RX", SDL_GAMEPAD_AXIS_RIGHTX }, { "RY", SDL_GAMEPAD_AXIS_RIGHTY },
                  { "LT", SDL_GAMEPAD_AXIS_LEFT_TRIGGER }, { "RT", SDL_GAMEPAD_AXIS_RIGHT_TRIGGER } },
      kMouse[] = { { "LEFT", 0 }, { "RIGHT", 1 } };
    auto find = [&](auto& table) {
        for (auto& e : table) {
            if (s == e.name) {
                return e.code;
            }
        }
        return -1;
    };
    switch (kind) {
        case 0:
            return find(kKeys);
        case 1:
            return find(kButtons);
        case 2:
            return find(kAxes);
        case 4:
            return find(kMouse);
        default:
            return 0;
    }
}

bool AttachVirtualPad() {
    SDL_VirtualJoystickDesc desc;
    SDL_INIT_INTERFACE(&desc);
    desc.type = SDL_JOYSTICK_TYPE_GAMEPAD;
    desc.naxes = SDL_GAMEPAD_AXIS_COUNT;
    desc.nbuttons = SDL_GAMEPAD_BUTTON_COUNT;
    desc.name = "T2 pad-test virtual gamepad";
    sVirtualId = SDL_AttachVirtualJoystick(&desc);
    if (sVirtualId == 0) {
        fprintf(stderr, "pad-test: SDL_AttachVirtualJoystick: %s\n", SDL_GetError());
        return false;
    }
    sVirtual = SDL_OpenJoystick(sVirtualId);
    printf("pad-test: virtual gamepad %u attached (is gamepad: %d)\n", (unsigned)sVirtualId,
           SDL_IsGamepad(sVirtualId) ? 1 : 0);
    return sVirtual != nullptr;
}

} // namespace

std::shared_ptr<Ship::ControllerDefaultMappings> T2Controls_DefaultMappings() {
    using Ship::DOWN;
    using Ship::LEFT;
    using Ship::LEFT_STICK;
    using Ship::RIGHT;
    using Ship::UP;
    // Keyboard. WASD walk/strafe (C buttons), the arrow keys turn and look (the stick, below),
    // Space jumps (R), Ctrl fires (Z; the mouse's left button too), Q/E the two weapon wheels
    // (A/B), Tab the map (L), Enter = START and A (title, pause, menu confirm), Esc = START
    // (pause), T/F/G/H the D-pad.
    std::unordered_map<CONTROLLERBUTTONS_T, std::unordered_set<KbScancode>> kbButtons = {
        { BTN_CUP, { KbScancode::LUS_KB_W } },
        { BTN_CDOWN, { KbScancode::LUS_KB_S } },
        { BTN_CLEFT, { KbScancode::LUS_KB_A } },
        { BTN_CRIGHT, { KbScancode::LUS_KB_D } },
        { BTN_R, { KbScancode::LUS_KB_SPACE } },
        { BTN_Z, { KbScancode::LUS_KB_CONTROL } },
        { BTN_A, { KbScancode::LUS_KB_Q, KbScancode::LUS_KB_ENTER } },
        { BTN_B, { KbScancode::LUS_KB_E } },
        { BTN_L, { KbScancode::LUS_KB_TAB } },
        { BTN_START, { KbScancode::LUS_KB_ENTER, KbScancode::LUS_KB_ESCAPE } },
        { BTN_DUP, { KbScancode::LUS_KB_T } },
        { BTN_DDOWN, { KbScancode::LUS_KB_G } },
        { BTN_DLEFT, { KbScancode::LUS_KB_F } },
        { BTN_DRIGHT, { KbScancode::LUS_KB_H } },
    };
    std::unordered_map<Ship::StickIndex, std::vector<std::pair<Ship::Direction, KbScancode>>> kbStick = {
        { LEFT_STICK,
          { { LEFT, KbScancode::LUS_KB_ARROWKEY_LEFT },
            { RIGHT, KbScancode::LUS_KB_ARROWKEY_RIGHT },
            { UP, KbScancode::LUS_KB_ARROWKEY_UP },
            { DOWN, KbScancode::LUS_KB_ARROWKEY_DOWN } } },
    };
    // Gamepad. South/East = A/B (menus confirm with A; hold for the weapon wheels, the right
    // stick picks), right trigger fires (Z), left trigger and right shoulder jump (R), left
    // shoulder and Back/Select the map (L), Start, the D-pad.
    std::unordered_map<CONTROLLERBUTTONS_T, std::unordered_set<SDL_GamepadButton>> padButtons = {
        { BTN_A, { SDL_GAMEPAD_BUTTON_SOUTH } },
        { BTN_B, { SDL_GAMEPAD_BUTTON_EAST } },
        { BTN_L, { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_BACK } },
        { BTN_R, { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER } },
        { BTN_START, { SDL_GAMEPAD_BUTTON_START } },
        { BTN_DUP, { SDL_GAMEPAD_BUTTON_DPAD_UP } },
        { BTN_DDOWN, { SDL_GAMEPAD_BUTTON_DPAD_DOWN } },
        { BTN_DLEFT, { SDL_GAMEPAD_BUTTON_DPAD_LEFT } },
        { BTN_DRIGHT, { SDL_GAMEPAD_BUTTON_DPAD_RIGHT } },
    };
    std::unordered_map<Ship::StickIndex, std::vector<std::pair<Ship::Direction, SDL_GamepadButton>>> padButtonStick = {
        { LEFT_STICK, {} },
    };
    // the left stick walks and strafes (the C buttons), the triggers fire and jump
    std::unordered_map<CONTROLLERBUTTONS_T, std::vector<std::pair<SDL_GamepadAxis, int32_t>>> padAxisButtons = {
        { BTN_CUP, { { SDL_GAMEPAD_AXIS_LEFTY, -1 } } },
        { BTN_CDOWN, { { SDL_GAMEPAD_AXIS_LEFTY, 1 } } },
        { BTN_CLEFT, { { SDL_GAMEPAD_AXIS_LEFTX, -1 } } },
        { BTN_CRIGHT, { { SDL_GAMEPAD_AXIS_LEFTX, 1 } } },
        { BTN_Z, { { SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, 1 } } },
        { BTN_R, { { SDL_GAMEPAD_AXIS_LEFT_TRIGGER, 1 } } },
    };
    // the right stick turns and looks (the N64 stick)
    std::unordered_map<Ship::StickIndex, std::vector<std::pair<Ship::Direction, std::pair<SDL_GamepadAxis, int32_t>>>>
        padAxisStick = {
            { LEFT_STICK,
              { { LEFT, { SDL_GAMEPAD_AXIS_RIGHTX, -1 } },
                { RIGHT, { SDL_GAMEPAD_AXIS_RIGHTX, 1 } },
                { UP, { SDL_GAMEPAD_AXIS_RIGHTY, -1 } },
                { DOWN, { SDL_GAMEPAD_AXIS_RIGHTY, 1 } } } },
        };
    return std::make_shared<LUS::ControllerDefaultMappings>(kbButtons, kbStick, padButtons, padButtonStick,
                                                            padAxisButtons, padAxisStick);
}

std::shared_ptr<Ship::Window> T2Controls_MakeHeadlessWindow(std::shared_ptr<Ship::ConsoleVariable> cvars) {
    // KeyboardGameInputBlocked() reads ImGui's context: give it an empty one
    if (ImGui::GetCurrentContext() == nullptr) {
        ImGui::CreateContext();
    }
    auto w = std::make_shared<HeadlessWindow>(std::move(cvars));
    // never destroyed: ~Window shuts ImGui down through a GUI that was never started
    new std::shared_ptr<Ship::Window>(w);
    return w;
}

void T2Controls_SetWindow(std::shared_ptr<Ship::Window> window) {
    sWindow = std::move(window);
}

void T2Controls_AfterContInit() {
    auto deck = ControllerGetControlDeck();
    if (deck == nullptr) {
        return;
    }
    if (sPadTest && !AttachVirtualPad()) {
        exit(1);
    }
    // SDL queues an "added" event for each pad present at start; T2Controls_PollHotplug takes
    // them (and refreshes) on the first read. Refresh now as well, for the first frame.
    deck->GetConnectedPhysicalDeviceManager()->RefreshConnectedSDLGamepads();
    int n = 0;
    SDL_JoystickID* ids = SDL_GetGamepads(&n);
    for (int i = 0; i < n; i++) {
        const char* name = SDL_GetGamepadNameForID(ids[i]);
        printf("controls: gamepad %d: %s\n", i + 1, name ? name : "?");
    }
    SDL_free(ids);
}

void T2Controls_PollHotplug() {
    SDL_PumpEvents();
    SDL_Event ev;
    bool changed = false;
    while (SDL_PeepEvents(&ev, 1, SDL_GETEVENT, SDL_EVENT_GAMEPAD_ADDED, SDL_EVENT_GAMEPAD_REMOVED) > 0) {
        if (ev.type == SDL_EVENT_GAMEPAD_ADDED || ev.type == SDL_EVENT_GAMEPAD_REMOVED) {
            const char* name = ev.type == SDL_EVENT_GAMEPAD_ADDED ? SDL_GetGamepadNameForID(ev.gdevice.which) : nullptr;
            printf("controls: gamepad %s: %u %s\n", ev.type == SDL_EVENT_GAMEPAD_ADDED ? "connected" : "disconnected",
                   (unsigned)ev.gdevice.which, name ? name : "");
            changed = true;
        }
    }
    if (changed) {
        if (auto deck = ControllerGetControlDeck(); deck != nullptr) {
            deck->GetConnectedPhysicalDeviceManager()->RefreshConnectedSDLGamepads();
        }
    }
}

void T2Controls_Mouse(uint16_t* buttons, int8_t* sx, int8_t* sy) {
    static bool swallow = false; // the click that captured the mouse does not fire
    if (sWindow == nullptr) {
        return;
    }
    const bool left = sWindow->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT);
    if (!sWindow->IsMouseCaptured()) {
        // a click in the window captures the mouse; F2 (LUS's Shortcuts.MouseCapture) toggles it
        if (left && !T2PadTest_Active()) {
            sWindow->SetMouseCapture(true);
            swallow = true;
        }
        return;
    }
    swallow = swallow && left;
    // look: the mouse's motion this frame as a stick deflection. CVars (turok2.cfg.json):
    // gT2MouseSensitivity (stick units per pixel, default 2), gT2MouseInvertY (0).
    const Ship::Coords d = sWindow->GetMouseDelta();
    const float sens = CVarGetFloat("gT2MouseSensitivity", 2.0f);
    const int invert = CVarGetInteger("gT2MouseInvertY", 0);
    if (d.x != 0 || d.y != 0) {
        const float x = std::clamp(d.x * sens, -80.0f, 80.0f);
        const float y = std::clamp((invert ? d.y : -d.y) * sens, -80.0f, 80.0f);
        if (d.x != 0) {
            *sx = (int8_t)std::lround(x);
        }
        if (d.y != 0) {
            *sy = (int8_t)std::lround(y);
        }
    }
    if (left && !swallow) {
        *buttons |= BTN_Z;
    }
    if (sWindow->GetMouseState(Ship::LUS_MOUSE_BTN_RIGHT)) {
        *buttons |= BTN_R;
    }
}

bool T2PadTest_Load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        fprintf(stderr, "--pad-test: cannot open %s\n", path.c_str());
        return false;
    }
    // FRAME key NAME down|up / FRAME btn NAME down|up / FRAME axis NAME VALUE (-32768..32767)
    // FRAME mouse DX DY / FRAME mbtn LEFT|RIGHT down|up / FRAME capture 0|1 / FRAME unplug / FRAME plug
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        lineNo++;
        if (auto h = line.find('#'); h != std::string::npos) {
            line.resize(h);
        }
        std::istringstream ss(line);
        PadEvent e = {};
        std::string kind, a, b;
        if (!(ss >> e.frame >> kind)) {
            continue;
        }
        ss >> a >> b;
        if (kind == "key" || kind == "btn" || kind == "mbtn") {
            e.kind = kind == "key" ? 0 : kind == "btn" ? 1 : 4;
            e.value = b == "down";
            if (b != "down" && b != "up") {
                e.code = -1;
            } else {
                e.code = ParseCode(e.kind, a);
            }
        } else if (kind == "axis") {
            e.kind = 2;
            e.code = ParseCode(2, a);
            e.value = atoi(b.c_str());
        } else if (kind == "mouse") {
            e.kind = 3;
            e.code = atoi(a.c_str());
            e.value = atoi(b.c_str());
        } else if (kind == "capture") {
            e.kind = 5;
            e.value = atoi(a.c_str());
        } else if (kind == "plug" || kind == "unplug") {
            e.kind = kind == "plug" ? 6 : 7;
        } else {
            e.kind = -1;
            e.code = -1;
        }
        if (e.kind != 3 && e.code < 0) {
            fprintf(stderr, "--pad-test: %s:%d: bad line\n", path.c_str(), lineNo);
            return false;
        }
        sPadEvents.push_back(e);
    }
    std::stable_sort(sPadEvents.begin(), sPadEvents.end(),
                     [](const PadEvent& a, const PadEvent& b) { return a.frame < b.frame; });
    sPadTest = true;
    return true;
}

bool T2PadTest_Active() {
    return sPadTest;
}

void T2PadTest_Apply(int frame) {
    if (!sPadTest) {
        return;
    }
    sMouseFeed.dx = sMouseFeed.dy = 0; // a motion lasts one read
    auto deck = ControllerGetControlDeck();
    // the game does not read the pad every VI: an event applies at the first read on or after
    // its frame (the file's order kept for one frame)
    static size_t next = 0;
    for (; next < sPadEvents.size() && sPadEvents[next].frame <= frame; next++) {
        const PadEvent& e = sPadEvents[next];
        switch (e.kind) {
            case 0:
                deck->ProcessKeyboardEvent(e.value ? Ship::LUS_KB_EVENT_KEY_DOWN : Ship::LUS_KB_EVENT_KEY_UP,
                                           (KbScancode)e.code);
                break;
            case 1:
                if (sVirtual) {
                    SDL_SetJoystickVirtualButton(sVirtual, e.code, e.value != 0);
                }
                break;
            case 2:
                if (sVirtual) {
                    SDL_SetJoystickVirtualAxis(sVirtual, e.code, (Sint16)std::clamp(e.value, -32768, 32767));
                }
                break;
            case 3:
                sMouseFeed.dx = e.code;
                sMouseFeed.dy = e.value;
                break;
            case 4:
                (e.code == 0 ? sMouseFeed.left : sMouseFeed.right) = e.value != 0;
                break;
            case 5:
                sMouseFeed.captured = e.value != 0;
                break;
            case 6:
                if (sVirtual == nullptr) {
                    AttachVirtualPad();
                }
                break;
            case 7:
                if (sVirtual != nullptr) {
                    SDL_CloseJoystick(sVirtual);
                    SDL_DetachVirtualJoystick(sVirtualId);
                    sVirtual = nullptr;
                    printf("pad-test: virtual gamepad detached\n");
                }
                break;
        }
    }
}
