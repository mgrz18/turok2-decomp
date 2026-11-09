// t2asset_lus.cpp - T2AssetBackend on libultraship's ArchiveManager.
//
// The engine-facing API stays port/assets/t2asset.h (T2Asset_Read / ReadInto / HostSize).
// This backend answers lookup/load from the archives LUS has mounted (the base turok2.o2r
// and any mod .o2r mounted after it, which shadow units by path). It reads raw files with
// ArchiveManager::LoadFile (no ResourceManager cache: the engine's CCache is the cache) and
// skips the 0x44-byte header (64-byte LUS resource header + the Blob's u32 size), the
// "Blob + skip header" route of docs/assets/ARCHIVE.md.
#include "t2asset_lus.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

#include "ship/resource/File.h"
#include "ship/resource/archive/ArchiveManager.h"

extern "C" {
#include "t2asset.h"
}

namespace {

constexpr uint32_t kLusHeader = 0x40;  // LUS resource header
constexpr uint32_t kUnitData = 0x44;   // + Blob u32 dataSize
constexpr uint32_t kTypeBlob = 0x4F424C42u;

uint32_t Le32(const uint8_t* p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
uint16_t Le16(const uint8_t* p) {
    return (uint16_t)(p[0] | (p[1] << 8));
}

struct Unit {
    uint32_t key, romSize, hostSize, flags;
};

struct LusCtx {
    std::shared_ptr<Ship::ArchiveManager> am;
    std::vector<Unit> units; // sorted by key (t2/units is written sorted)
    char err[256] = {};
};

LusCtx* gCtx = nullptr;

const Unit* Find(const LusCtx* c, uint32_t key) {
    auto it = std::lower_bound(c->units.begin(), c->units.end(), key,
                               [](const Unit& u, uint32_t k) { return u.key < k; });
    return (it != c->units.end() && it->key == key) ? &*it : nullptr;
}

int LusLookup(void* ctx, uint32_t key, uint32_t* rs, uint32_t* hs, uint32_t* fl) {
    const Unit* u = Find(static_cast<LusCtx*>(ctx), key);
    if (u == nullptr) {
        return 0;
    }
    *rs = u->romSize, *hs = u->hostSize, *fl = u->flags;
    return 1;
}

int LusLoad(void* ctx, uint32_t key, void* dst, uint32_t hostSize) {
    auto* c = static_cast<LusCtx*>(ctx);
    char path[32];
    snprintf(path, sizeof(path), "t2/u/%08X", key);
    auto file = c->am->LoadFile(path);
    if (file == nullptr || !file->IsLoaded || file->Buffer == nullptr) {
        snprintf(c->err, sizeof(c->err), "%s: not found by ArchiveManager", path);
        return -1;
    }
    const auto& buf = *file->Buffer;
    const auto* p = reinterpret_cast<const uint8_t*>(buf.data()) + file->BufferOffset;
    const size_t n = buf.size() - file->BufferOffset;
    if (n < kUnitData || p[0] != 0 || Le32(p + 4) != kTypeBlob) {
        snprintf(c->err, sizeof(c->err), "%s: not a little-endian Blob resource", path);
        return -1;
    }
    // A mod may replace a unit with a different size: the entry's own Blob size wins
    // (ARCHIVE.md "Mods"); the caller sized dst from lookup(), so it must agree here.
    const uint32_t blobSize = Le32(p + kLusHeader);
    if (blobSize != hostSize || n < kUnitData + (size_t)blobSize ||
        (memcmp(p + 0x14, "T2LU", 4) == 0 && (Le32(p + 0x18) != key || Le32(p + 0x20) != hostSize))) {
        snprintf(c->err, sizeof(c->err), "%s: entry size %u disagrees with t2/units (%u)", path, blobSize, hostSize);
        return -1;
    }
    memcpy(dst, p + kUnitData, hostSize);
    return 0;
}

uint32_t LusCount(void* ctx) {
    return (uint32_t) static_cast<LusCtx*>(ctx)->units.size();
}
uint32_t LusKeyAt(void* ctx, uint32_t i) {
    return static_cast<LusCtx*>(ctx)->units[i].key;
}
void LusClose(void* ctx) {
    auto* c = static_cast<LusCtx*>(ctx);
    if (c == gCtx) {
        gCtx = nullptr;
    }
    delete c;
}

const T2AssetBackend kLusBackend = {
    "libultraship", LusLookup, LusLoad, nullptr, LusCount, LusKeyAt, LusClose,
};

} // namespace

int T2Asset_OpenLus(std::shared_ptr<Ship::ArchiveManager> am, std::string* error) {
    auto fail = [&](const std::string& why) {
        if (error) {
            *error = why;
        }
        return -1;
    };
    if (am == nullptr || !am->IsInitialized()) {
        return fail("ArchiveManager not initialized");
    }
    auto file = am->LoadFile("t2/units");
    if (file == nullptr || !file->IsLoaded) {
        return fail("t2/units missing (not a Turok 2 archive?)");
    }
    const auto* p = reinterpret_cast<const uint8_t*>(file->Buffer->data()) + file->BufferOffset;
    const size_t n = file->Buffer->size() - file->BufferOffset;
    if (n < 16 || memcmp(p, "T2UT", 4) != 0) {
        return fail("t2/units: bad magic");
    }
    const uint16_t major = Le16(p + 4);
    const uint32_t count = Le32(p + 8);
    if (major != 1 || n < 16 + (size_t)count * 16) {
        return fail("t2/units: unsupported version or truncated");
    }
    auto* c = new LusCtx;
    c->am = std::move(am);
    c->units.reserve(count);
    for (uint32_t i = 0; i < count; i++) {
        const uint8_t* r = p + 16 + i * 16;
        c->units.push_back({ Le32(r), Le32(r + 4), Le32(r + 8), Le16(r + 14) });
    }
    if (!std::is_sorted(c->units.begin(), c->units.end(),
                        [](const Unit& a, const Unit& b) { return a.key < b.key; })) {
        delete c;
        return fail("t2/units: not sorted");
    }
    gCtx = c;
    T2Asset_SetBackend(&kLusBackend, c);
    return 0;
}

const char* T2Asset_LusError(void) {
    return gCtx ? gCtx->err : "";
}
