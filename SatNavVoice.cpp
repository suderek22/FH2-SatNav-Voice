#include "fh2mod.h"

#include <windows.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

constexpr wchar_t kSettingsFile[] = L"SatNavVoice.ini";
constexpr wchar_t kSettingSection[] = L"SatNavVoice";
constexpr wchar_t kSettingName[] = L"CustomVoice";
constexpr char kMenuSettingKey[] = "CustomVoice";
constexpr char kMenuSettingLabel[] = "SatNav Voice - PL";
constexpr char kMenuSettingDescription[] =
    "Use the SatNav Voice Polish replacement. Restart the game for changes to take effect.";
constexpr char kBankMediaPath[] = "audio/fmodanthem/durango/HZ2_DX_SatNav_PL.bank";

std::filesystem::path g_settings_path;

bool module_directory(std::filesystem::path& directory) {
    std::wstring module_path(32768, L'\0');
    const DWORD length = GetModuleFileNameW(
        reinterpret_cast<HMODULE>(&__ImageBase), module_path.data(),
        static_cast<DWORD>(module_path.size()));
    if (length == 0 || length >= module_path.size()) {
        return false;
    }
    module_path.resize(length);
    directory = std::filesystem::path(module_path).parent_path();
    return !directory.empty();
}

bool read_file(const std::filesystem::path& path, std::vector<std::uint8_t>& bytes) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    bytes.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return !bytes.empty();
}

// A RIFF/FEV bank whose SND chunk holds an FSB5 container.
bool valid_bank(const std::vector<std::uint8_t>& b) {
    if (b.size() < 64 || std::memcmp(b.data(), "RIFF", 4) != 0 ||
        std::memcmp(b.data() + 8, "FEV ", 4) != 0) {
        return false;
    }
    std::uint32_t riff_size = 0;
    std::memcpy(&riff_size, b.data() + 4, 4);
    if (static_cast<std::size_t>(riff_size) + 8 != b.size()) {
        return false;
    }
    for (std::size_t pos = 12; pos + 8 <= b.size();) {
        std::uint32_t size = 0;
        std::memcpy(&size, b.data() + pos + 4, 4);
        if (std::memcmp(b.data() + pos, "SND ", 4) == 0) {
            return pos + 8 + size <= b.size() && size > 32 &&
                   std::memcmp(b.data() + pos + 8 + 18, "FSB5", 4) == 0;
        }
        pos += 8 + static_cast<std::size_t>(size);
    }
    return false;
}

bool read_boolean_setting(const std::filesystem::path& settings_path, bool default_value,
                          bool& enabled) {
    wchar_t value[16]{};
    const DWORD length = GetPrivateProfileStringW(
        kSettingSection, kSettingName, L"", value, 16, settings_path.c_str());
    if (length == 0) {
        if (!WritePrivateProfileStringW(kSettingSection, kSettingName,
                                        default_value ? L"1" : L"0",
                                        settings_path.c_str())) {
            return false;
        }
        enabled = default_value;
        return true;
    }
    if (std::wcscmp(value, L"0") == 0) {
        enabled = false;
        return true;
    }
    if (std::wcscmp(value, L"1") == 0) {
        enabled = true;
        return true;
    }
    return false;
}

void on_setting_changed(void*, const char* key, int value) {
    if (!key || std::strcmp(key, kMenuSettingKey) != 0 || (value != 0 && value != 1)) {
        return;
    }
    WritePrivateProfileStringW(kSettingSection, kSettingName, value == 1 ? L"1" : L"0",
                               g_settings_path.c_str());
}

} // namespace

FH2MOD_EXPORT int fh2mod_init(const FH2ModApi* api) {
    if (!api || api->api_version < 2 || !api->register_mod || !api->clear_file ||
        !api->set_file) {
        return 1;
    }

    FH2ModInfo info{};
    info.size = sizeof(info);
    info.name = "SatNavVoice";
    info.version = "1.0.0";
    info.github_repo = nullptr;
    info.platform_version = "2.7.1";
    info.check_updates = 0;
    info.description = "Replaces all Polish SatNav voice lines with a custom Polish voice.";

    FH2Mod* mod = api->register_mod(&info);
    if (!mod) {
        return 1;
    }

    std::filesystem::path mod_directory;
    if (!module_directory(mod_directory)) {
        api->clear_file(mod, nullptr);
        api->log(mod, "could not locate the mod DLL directory; overrides cleared");
        return 1;
    }

    g_settings_path = mod_directory / kSettingsFile;
    bool enabled = false;
    if (!read_boolean_setting(g_settings_path, true, enabled)) {
        api->clear_file(mod, nullptr);
        api->log(mod, "invalid CustomVoice setting; overrides cleared");
        return 0;
    }

    if (api->api_version >= 4 && api->add_setting) {
        FH2Setting setting{};
        setting.size = sizeof(setting);
        setting.key = kMenuSettingKey;
        setting.label = kMenuSettingLabel;
        setting.description = kMenuSettingDescription;
        setting.value = enabled ? 1 : 0;
        setting.needs_restart = 1;
        setting.on_change = on_setting_changed;
        if (!api->add_setting(mod, &setting)) {
            api->log(mod, "could not add the option to the in-game Mods menu");
        }
    }

    api->clear_file(mod, nullptr);
    if (!enabled) {
        api->log(mod, "CustomVoice=0; stock SatNav voice is unchanged");
        return 0;
    }

    std::vector<std::uint8_t> bank;
    if (!read_file(mod_directory / L"SatNav" / L"HZ2_DX_SatNav_PL.bank", bank) ||
        !valid_bank(bank)) {
        api->log(mod, "SatNav\\HZ2_DX_SatNav_PL.bank is missing or invalid; no overrides applied");
        return 0;
    }

    if (!api->set_file(mod, kBankMediaPath, bank.data(), bank.size())) {
        api->clear_file(mod, nullptr);
        api->log(mod, "failed to install the custom SatNav bank; overrides cleared");
        return 0;
    }

    api->log(mod, "installed SatNav Voice - PL bank (122 of 122 lines replaced)");
    return 0;
}
