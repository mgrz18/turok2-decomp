// main.cpp - Turok 2 native port: host executable skeleton on libultraship (Phase 7, p3).
//
// What it does today (no game code is linked yet):
//   1. builds a LUS epoch-2 Context by hand (Logger, Config, ConsoleVariable, ThreadPool,
//      ResourceManager, ControlDeck, Console, CrashHandler, Fast3dWindow, FileDrop, Events),
//      registers the bridge caches (the C bridge functions game code will call) and the
//      Blob factory;
//   2. mounts turok2.o2r through LUS's ArchiveManager, checks the `version` file / game
//      version, installs the T2Asset backend on the ArchiveManager (t2asset_lus.cpp) and
//      reads every load unit through it (optionally comparing with the miniz archive
//      reader and the ROM reader of port/assets/t2asset.c); reads a few units through the
//      ResourceManager as Blob resources too;
//   3. opens the window (SDL3 + OpenGL; headless: SDL_VIDEODRIVER=offscreen or Xvfb), polls
//      the controller deck, runs N frames of a test display list through Fast3D
//      (DrawAndRunGraphicsCommands, F3DEX2 ucode) and reads a pixel back;
//   4. exits with 0 when every check passed.
//
//   turok2 [--archive turok2.o2r] [--port-archive t2port.o2r] [--rom baserom.us.z64] [--frames N] [--no-window]
//
// With --game (when the game is linked, T2_HAVE_GAME; docs/port/LINK.md) step 2 only mounts
// the archive and installs the T2Asset backend, and step 3 runs the game (game.cpp) instead of
// the test display list: [--draw] hands its display lists to Fast3D, [--verbose] logs every
// frame and task, [--high-memory] turns off the low-memory bring-up aid.
#include <libultraship/libultraship.h>

#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include <SDL3/SDL.h>

#include "fast/Fast3dWindow.h"
#include "fast/debug/GfxDebugger.h"
#include "fast/interpreter.h"
#include "fast/backends/gfx_rendering_api.h"
#include "fast/ucodehandlers.h"
#include "libultraship/controller/controldeck/ControlDeck.h"
#include "ship/config/Config.h"
#include "ship/config/ConsoleVariable.h"
#include "ship/core/Context.h"
#include "ship/debug/Console.h"
#include "ship/debug/CrashHandler.h"
#include "ship/events/Events.h"
#include "ship/log/Logger.h"
#include "ship/resource/ResourceManager.h"
#include "ship/resource/ResourceLoader.h"
#include "ship/resource/archive/ArchiveManager.h"
#include "ship/resource/factory/BlobFactory.h"
#include "ship/resource/type/Blob.h"
#include "ship/thread/ThreadPool.h"
#include "ship/window/FileDrop.h"

#include "t2asset_lus.h"
#ifdef T2_HAVE_GAME
#include "game.h"
#endif
extern "C" {
#include "t2asset.h"
extern Gfx gT2TestDL[];
}

namespace {

constexpr uint32_t kGameVersionUS = 0x49088A11; // ROM header CRC1, the archive's `version`

int gFailures = 0;
#define CHECK(cond, ...)                                                                                     \
    do {                                                                                                     \
        if (!(cond)) {                                                                                       \
            fprintf(stderr, "FAIL: " __VA_ARGS__);                                                           \
            fputc('\n', stderr);                                                                             \
            gFailures++;                                                                                     \
        }                                                                                                    \
    } while (0)

uint64_t Fnv1a(uint64_t h, const void* data, size_t n) {
    const auto* p = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < n; i++) {
        h = (h ^ p[i]) * 0x100000001B3ull;
    }
    return h;
}

struct Pass {
    uint32_t units = 0, failed = 0;
    uint64_t bytes = 0, hash = 0xCBF29CE484222325ull;
    double seconds = 0;
};

// Read every unit through the current T2Asset backend (whole units: size 0).
Pass ReadAllUnits() {
    Pass r;
    auto t0 = std::chrono::steady_clock::now();
    const uint32_t n = T2Asset_UnitCount();
    for (uint32_t i = 0; i < n; i++) {
        uint32_t key = T2Asset_UnitKey(i), size = 0;
        const void* p = T2Asset_Read(key, &size);
        if (p == nullptr) {
            if (r.failed++ < 5) {
                fprintf(stderr, "  %s: unit %08X: %s\n", T2Asset_BackendName(), key, T2Asset_Error());
            }
            continue;
        }
        r.hash = Fnv1a(r.hash, &key, 4);
        r.hash = Fnv1a(r.hash, p, size);
        r.bytes += size;
        r.units++;
        T2Asset_Release(p);
    }
    r.seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    return r;
}

void PrintPass(const char* what, const Pass& p) {
    printf("  %-13s %u units, %" PRIu64 " bytes, fnv1a64 %016" PRIx64 ", %u failed, %.2f s\n", what, p.units,
           p.bytes, p.hash, p.failed, p.seconds);
}

struct Args {
    std::string archive = "turok2.o2r";
    std::string portArchive = T2_PORT_O2R_DEFAULT;
    std::string rom;
    int frames = 3;
    bool window = true;
    bool game = false;
    bool draw = false;
    bool audio = false;
    std::string wav;      // --wav <file>: the game's mixed audio output as a WAV (implies --audio)
    bool verbose = false;
#ifdef T2_LOW_MEMORY_DEFAULT
    bool lowMemory = true;
#else
    bool lowMemory = false;
#endif
};

Args ParseArgs(int argc, char** argv) {
    Args a;
    for (int i = 1; i < argc; i++) {
        std::string s = argv[i];
        if (s == "--archive" && i + 1 < argc) {
            a.archive = argv[++i];
        } else if (s == "--port-archive" && i + 1 < argc) {
            a.portArchive = argv[++i];
        } else if (s == "--rom" && i + 1 < argc) {
            a.rom = argv[++i];
        } else if (s == "--frames" && i + 1 < argc) {
            a.frames = atoi(argv[++i]);
        } else if (s == "--no-window") {
            a.window = false;
        } else if (s == "--game") {
            a.game = true;
        } else if (s == "--draw") {
            a.draw = true;
        } else if (s == "--audio") {
            a.audio = true;
        } else if (s == "--wav" && i + 1 < argc) {
            a.wav = argv[++i];
            a.audio = true;
        } else if (s == "--verbose") {
            a.verbose = true;
        } else if (s == "--high-memory") {
            a.lowMemory = false;
        } else {
            fprintf(stderr, "usage: %s [--archive turok2.o2r] [--port-archive t2port.o2r] [--rom baserom.us.z64] [--frames N] [--no-window]\n"
                            "          [--game [--draw] [--audio] [--wav out.wav] [--verbose] [--high-memory]]\n",
                    argv[0]);
            exit(2);
        }
    }
    return a;
}

} // namespace

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IOLBF, 0);
    Args args = ParseArgs(argc, argv);
    const std::string archivePath = std::filesystem::absolute(args.archive).string();

    // ---- 1. Context and components (LUS epoch 2, "manual graph" style as in Ghostship) ----
    auto context = Ship::Context::CreateInstance("Turok2", "Turok 2: Seeds of Evil");
    context->Init();

    auto logger = std::make_shared<Ship::Logger>(
        "Turok2", Ship::Context::GetPathRelativeToAppDirectory("logs/turok2.log"));
    context->GetChildren().Add(logger);
    logger->Init();

    auto config = std::make_shared<Ship::Config>(Ship::Context::GetPathRelativeToAppDirectory("turok2.cfg.json"));
    context->GetChildren().Add(config);
    auto cvars = std::make_shared<Ship::ConsoleVariable>(config);
    context->GetChildren().Add(cvars);
    CVarSetConsoleVariable(cvars);

    // Only constructed when it will be initialized: ~Fast3dWindow calls the interpreter's
    // Destroy(), which dereferences the window backend that only Init() creates.
    std::shared_ptr<Fast::Fast3dWindow> window;
    if (args.window) {
        window = std::make_shared<Fast::Fast3dWindow>(std::vector<std::shared_ptr<Ship::GuiWindow>>{}, config, cvars);
    }

    auto threadPool = std::make_shared<Ship::ThreadPool>(2);
    context->GetChildren().Add(threadPool);
    auto resourceManager = std::make_shared<Ship::ResourceManager>(threadPool);
    context->GetChildren().Add(resourceManager);
    ResourceSetResourceManager(resourceManager);

    auto controlDeck = std::make_shared<LUS::ControlDeck>(window, cvars);
    context->GetChildren().Add(controlDeck);
    ControllerSetControlDeck(controlDeck);

    {
        nlohmann::json rmArgs;
        // Mount order = priority (later shadows earlier): the port archive (shaders), the
        // game archive, then mods. validHashes stays empty: LUS would reject every archive
        // whose `version` is not listed, and t2port.o2r is ROM independent and has none;
        // the game archive's version is checked below instead (Ghostship does the same).
        rmArgs["archivePaths"] =
            std::vector<std::string>{ std::filesystem::absolute(args.portArchive).string(), archivePath };
        rmArgs["validHashes"] = std::vector<uint32_t>{};
        try {
            resourceManager->Init(rmArgs);
        } catch (const std::exception& e) {
            fprintf(stderr, "ResourceManager init failed: %s\n", e.what());
            return 1;
        }
    }
    auto archives = resourceManager->GetArchiveManager();
    if (archives == nullptr || !archives->IsInitialized()) {
        fprintf(stderr, "FAIL: %s not mounted by ArchiveManager\n", archivePath.c_str());
        return 1;
    }

    auto console = std::make_shared<Ship::Console>();
    context->GetChildren().Add(console);
    console->Init();
    auto crashHandler = std::make_shared<Ship::CrashHandler>();
    context->GetChildren().Add(crashHandler);
    CrashHandlerSetComponent(crashHandler);
    auto events = std::make_shared<Ship::Events>();
    context->GetChildren().Add(events);
    EventSystemSetEvents(events);

    // Blob is not a global factory in LUS 2.0 (only Json and Shader are): every port
    // registers it (Ghostship/SpaghettiKart/Starship Engine.cpp).
    resourceManager->GetResourceLoader()->RegisterResourceFactory(
        std::make_shared<Ship::ResourceFactoryBinaryBlobV0>(), RESOURCE_FORMAT_BINARY, "Blob",
        static_cast<uint32_t>(Ship::ResourceType::Blob), 0);

    // ---- 2. The archive through LUS ----
    printf("archive: %s\n", archivePath.c_str());
    {
        auto versions = archives->GetGameVersions();
        printf("  mounted archives: %zu, game versions:", archives->GetArchives()->size());
        for (auto v : versions) {
            printf(" %08X", v);
        }
        printf("\n");
        int gameArchives = 0;
        auto mounted = archives->GetArchives(); // keep the shared_ptr alive across the loop
        for (auto& a : *mounted) {
            printf("    %s: %s\n", a->GetPath().c_str(),
                   a->HasGameVersion() ? (a->GetGameVersion() == kGameVersionUS ? "Turok 2 US (49088A11)" : "OTHER ROM")
                                       : "no game version (port archive)");
            gameArchives += a->HasGameVersion() && a->GetGameVersion() == kGameVersionUS;
            CHECK(!a->HasGameVersion() || a->GetGameVersion() == kGameVersionUS, "%s is for another ROM",
                  a->GetPath().c_str());
        }
        CHECK(gameArchives == 1, "expected one Turok 2 US game archive, found %d", gameArchives);

        auto ver = archives->LoadFile("version");
        CHECK(ver && ver->IsLoaded && ver->Buffer->size() >= 0x21, "version file");
        if (ver && ver->IsLoaded && ver->Buffer->size() >= 0x21) {
            const auto* v = reinterpret_cast<const uint8_t*>(ver->Buffer->data());
            uint32_t crc;
            memcpy(&crc, v + 1, 4);
            printf("  version: byteOrder %u, gameVersion %08X, ext '%.4s' %u.%u\n", v[0], crc, v + 5,
                   v[9] | (v[10] << 8), v[11] | (v[12] << 8));
            CHECK(v[0] == 0 && crc == kGameVersionUS && memcmp(v + 5, "T2AV", 4) == 0, "version contents");
        }
        auto files = archives->ListFiles("t2/u/*");
        printf("  ListFiles(t2/u/*): %zu entries\n", files->size());
    }

    std::string err;
    if (T2Asset_OpenLus(archives, &err) != 0) {
        fprintf(stderr, "FAIL: T2Asset_OpenLus: %s\n", err.c_str());
        return 1;
    }
    printf("T2Asset over ArchiveManager: %u units in t2/units, byte swap %s\n", T2Asset_UnitCount(),
           T2Asset_SwapActive() ? "on (t2/swap plans, little-endian host)" : "off");
#ifndef T2_HAVE_GAME
    if (args.game) {
        fprintf(stderr, "--game: this build does not link the game (T2_LINK_GAME=OFF)\n");
        return 2;
    }
#endif
    if (!args.game) {

    // A few named units, the two routes of ARCHIVE.md: raw LoadFile + skip 0x44 (what
    // T2Asset uses) and the ResourceManager's Blob resource (+16 bytes of padding).
    const uint32_t sample[] = { 0x00FCEA9Cu /* object 0 geometry */, T2_ROOT_CARTRIDGE, T2_ROOT_AUDIO_CTL,
                                T2Asset_UnitKey(0), T2Asset_UnitKey(T2Asset_UnitCount() / 2),
                                T2Asset_UnitKey(T2Asset_UnitCount() - 1) };
    for (uint32_t key : sample) {
        uint32_t rs = 0, hs = 0, fl = 0, size = 0;
        if (!T2Asset_UnitInfo(key, &rs, &hs, &fl)) {
            CHECK(false, "unit %08X not in t2/units", key);
            continue;
        }
        const auto* data = static_cast<const uint8_t*>(T2Asset_Read(key, &size));
        CHECK(data != nullptr && size == hs, "T2Asset_Read(%08X): %s", key, T2Asset_LusError());
        char path[32];
        snprintf(path, sizeof(path), "t2/u/%08X", key);
        auto blob = std::dynamic_pointer_cast<Ship::Blob>(ResourceLoad(path));
        size_t blobSize = ResourceGetSizeByName(path);
        // the Blob holds the stored (ROM-order) bytes: through the unit's swap plan they must
        // equal what T2Asset hands the engine
        std::vector<uint8_t> host;
        if (blob && blob->Data.size() >= hs) {
            host.assign(blob->Data.begin(), blob->Data.begin() + hs);
            T2Asset_SwapUnit(key, host.data(), hs);
        }
        bool same = blob && data && blob->Data.size() == (size_t)hs + 16 && memcmp(host.data(), data, hs) == 0;
        CHECK(same, "Blob route differs for %s", path);
        CHECK(blobSize == (size_t)hs + 16, "ResourceGetSizeByName(%s) = %zu, want %u + 16", path, blobSize, hs);
        printf("  %s rom %6X host %6X flags %03X first bytes %02X%02X%02X%02X  blob: %s (size %zu)\n", path, rs, hs,
               fl, data && hs > 0 ? data[0] : 0, data && hs > 1 ? data[1] : 0, data && hs > 2 ? data[2] : 0,
               data && hs > 3 ? data[3] : 0, same ? "same bytes" : "DIFFERENT", blobSize);
        if (data) {
            T2Asset_Release(data);
        }
    }
    // Inner-address read (audio wave table, __amDMA's 0x300-byte windows).
    {
        uint8_t buf[0x300];
        int rc = T2Asset_ReadInto(T2_ROOT_AUDIO_DATA + 0x1000, 0, buf, sizeof(buf));
        CHECK(rc == 0, "T2Asset_ReadInto(audio data + 0x1000): %s", T2Asset_Error());
    }

    // Every unit through LUS, then (as a cross-check) through the miniz archive reader and
    // the ROM reader of t2asset.c: the three backends must return identical bytes.
    printf("reading every unit:\n");
    Pass lus = ReadAllUnits();
    PrintPass("libultraship", lus);
    CHECK(lus.failed == 0 && lus.units == T2Asset_UnitCount(), "LUS pass");
    T2Asset_Close();

    if (T2Asset_OpenArchive(archivePath.c_str()) == 0) {
        Pass mz = ReadAllUnits();
        PrintPass("miniz", mz);
        CHECK(mz.hash == lus.hash && mz.bytes == lus.bytes, "miniz and LUS backends disagree");
        T2Asset_Close();
    } else {
        CHECK(false, "T2Asset_OpenArchive: %s", T2Asset_Error());
    }
    if (!args.rom.empty()) {
        // t2/units.tsv from the archive (via LUS) gives the ROM backend its unit list.
        auto tsv = archives->LoadFile("t2/units.tsv");
        std::string tsvPath = (std::filesystem::temp_directory_path() / "t2_units_from_o2r.tsv").string();
        if (tsv && tsv->IsLoaded) {
            std::ofstream(tsvPath, std::ios::binary).write(tsv->Buffer->data(), tsv->Buffer->size());
        }
        if (T2Asset_OpenRom(args.rom.c_str(), tsvPath.c_str()) == 0 &&
            T2Asset_LoadSwapFile(archivePath.c_str()) == 0) { // the same host-order bytes
            Pass rom = ReadAllUnits();
            PrintPass("rom", rom);
            CHECK(rom.hash == lus.hash && rom.bytes == lus.bytes, "ROM and LUS backends disagree");
            T2Asset_Close();
        } else {
            CHECK(false, "T2Asset_OpenRom: %s", T2Asset_Error());
        }
        std::filesystem::remove(tsvPath);
    }
    } // !args.game: the backend stays open for the game

    // ---- 3. Window, controller, Fast3D ----
    if (args.window) {
        context->GetChildren().Add(window);
        WindowSetWindowComponent(window);
        GfxSetFast3dWindow(window);
        auto fileDrop = std::make_shared<Ship::FileDrop>(window);
        context->GetChildren().Add(fileDrop);
        window->Init();
        fileDrop->Init();
        window->SetRendererUCode(ucode_f3dex2); // "F3DEX.NoN fifo 2.05" = F3DEX2 (include/globals/ucode.h)
        window->SetTargetFps(60);
        window->SetMaximumFrameLatency(1);
        printf("window: backend %s, video driver %s, %ux%u\n", window->GetWindowBackendName().c_str(),
               SDL_GetCurrentVideoDriver() ? SDL_GetCurrentVideoDriver() : "?", window->GetWidth(),
               window->GetHeight());
    }
#ifdef T2_HAVE_GAME
    if (args.game) {
        T2GameArgs g;
        g.rom = args.rom;
        g.frames = args.frames;
        g.draw = args.draw;
        g.audio = args.audio;
        g.wav = args.wav;
        g.lowMemory = args.lowMemory;
        g.verbose = args.verbose;
        CHECK(T2Game_Run(window, g) == 0, "game run");
    }
#endif
    if (args.window && !args.game) {

        // Controllers through LUS's libultra replacements, as the game's own code will call
        // them: osContInit initializes SDL gamepads and the ControlDeck,
        // osContStartReadData is a no-op, osContGetReadData fills OSContPad from the deck.
        OSMesgQueue contQueue;
        OSMesg contMsg[1];
        osCreateMesgQueue(&contQueue, contMsg, 1);
        uint8_t contBits = 0;
        OSContStatus contStatus[MAXCONTROLLERS] = {};
        CHECK(osContInit(&contQueue, &contBits, contStatus) == 0, "osContInit");
        printf("  osContInit: controller bits %02X\n", contBits);

        OSContPad pads[MAXCONTROLLERS] = {};
        int drawn = 0;
        for (int f = 0; f < args.frames && window->IsRunning(); f++) {
            window->HandleEvents();
            osContStartReadData(&contQueue);
            osContGetReadData(pads);
            if (window->DrawAndRunGraphicsCommands(gT2TestDL, {})) {
                drawn++;
            }
        }
        printf("  frames: %d requested, %d drawn; pad 0 buttons %04X stick %d,%d\n", args.frames, drawn,
               pads[0].button, pads[0].stick_x, pads[0].stick_y);
        CHECK(drawn > 0, "no frame drawn");

        // Read the rendered frame back: the fill rectangles must be there.
        auto interp = window->GetInterpreterWeak().lock();
        if (interp && interp->mRapi && drawn > 0) {
            const uint32_t w = interp->mCurDimensions.width, h = interp->mCurDimensions.height;
            std::vector<uint16_t> fb((size_t)w * h);
            // Without the ImGui game viewport Fast3D draws straight into the window (fb 0);
            // mGameFb is only the target when the game is shown inside a GUI window.
            int fbId = 0;
            interp->mRapi->ReadFramebufferToCPU(fbId, w, h, fb.data());
            auto rgb = [](uint16_t c) { return ((c >> 11 & 31) << 16) | ((c >> 6 & 31) << 8) | (c >> 1 & 31); };
            uint32_t centre = rgb(fb[(size_t)(h / 2) * w + w / 2]), corner = rgb(fb[(size_t)2 * w + 2]);
            printf("  framebuffer #%d %ux%u (native %ux%u): centre %06X corner %06X (5-bit channels)\n", fbId, w, h,
                   interp->mNativeDimensions.width, interp->mNativeDimensions.height, centre, corner);
            CHECK(centre == 0x1F1600 && corner == 0x040C04, "framebuffer does not hold the test rectangles");
            // Screenshot (PPM, top row first) next to the config, for people.
            std::string ppm = Ship::Context::GetPathRelativeToAppDirectory("turok2_testdl.ppm");
            if (FILE* fp = fopen(ppm.c_str(), "wb")) {
                fprintf(fp, "P6\n%u %u\n255\n", w, h);
                for (uint32_t y = 0; y < h; y++) {
                    for (uint32_t x = 0; x < w; x++) {
                        uint16_t c = fb[(size_t)(h - 1 - y) * w + x];
                        uint8_t px[3] = { (uint8_t)((c >> 11 & 31) << 3), (uint8_t)((c >> 6 & 31) << 3),
                                          (uint8_t)((c >> 1 & 31) << 3) };
                        fwrite(px, 1, 3, fp);
                    }
                }
                fclose(fp);
                printf("  wrote %s\n", ppm.c_str());
            }
        }
    }

    printf("%s (%d failed checks)\n", gFailures ? "FAILED" : "OK", gFailures);
    fflush(stdout);
    // Shutdown. The bridge setters above keep shared_ptrs in statics; without a Ship::Bridge
    // component the Context does not clear them, and destroying them during static
    // destruction (after spdlog is gone) crashes in ~CrashHandler. Clear them first.
    if (args.window) {
        window->Close();
    }
    GfxSetFast3dWindow(nullptr);
    WindowSetWindowComponent(nullptr);
    EventSystemSetEvents(nullptr);
    CrashHandlerSetComponent(nullptr);
    ControllerSetControlDeck(nullptr);
    ResourceSetResourceManager(nullptr);
    CVarSetConsoleVariable(nullptr);
    context.reset();
    return gFailures ? 1 : 0;
}
