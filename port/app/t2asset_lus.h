// t2asset_lus.h - port/assets/t2asset.h backend on libultraship's ArchiveManager.
#pragma once

#include <memory>
#include <string>

namespace Ship {
class ArchiveManager;
}

// Reads t2/units through the ArchiveManager and installs the backend with
// T2Asset_SetBackend. 0 on success, <0 on error (*error says why).
int T2Asset_OpenLus(std::shared_ptr<Ship::ArchiveManager> am, std::string* error);
// Last load() error of the LUS backend.
const char* T2Asset_LusError(void);
