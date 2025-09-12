#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace engine::core {

Config::Config(const std::string& filename)
{
    loadFromFile(filename);
}

bool Config::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        spdlog::warn("Failed to open config file: {}", filename);
        return false;
    }
    return true;

    try {
        nlohmann::json j;
        file >> j;
        fromJson(j);
        spdlog::info("Config loaded from file: {}", filename);
        return true;
    } catch (const std::exception& e) {
        spdlog::error("Failed to load config from file: {}. Error: {}", filename, e.what());
    }
    return false;
}

bool Config::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if(!file.is_open()){
        spdlog::warn("Failed to open config file: {}", filename);
        return false;
    }

    try{
        nlohmann::ordered_json j = toJson();
        file << j.dump(4);
        spdlog::info("Config saved to file: {}", filename);
        return true;
    }catch(const std::exception& e){
        spdlog::error("Failed to save config to file: {}. Error: {}", filename, e.what());
    }
    return false;
}

void Config::fromJson(const nlohmann::json& j)
{
    if(j.contains("window")){
        const auto&window_config = j["window"];
        _window_title = window_config.value("title", "SunnyLand");
        _window_width = window_config.value("width", 1920);
        _window_height = window_config.value("height", 1080);
        _window_resizable = window_config.value("resizable", true);
    }
    if (j.contains("graphics")) {
        const auto& graphics_config = j["graphics"];
        _vsync_enabled = graphics_config.value("vsync", _vsync_enabled);
    }
    if (j.contains("performance")) {
        const auto& perf_config = j["performance"];
        _target_fps = perf_config.value("target_fps", _target_fps);
        if (_target_fps < 0) {
            spdlog::warn("目标 FPS 不能为负数。设置为 0(无限制)。");
            _target_fps = 0;
        }
    }
    if (j.contains("audio")) {
        const auto& audio_config = j["audio"];
        _music_volume = audio_config.value("music_volume", _music_volume);
        _sound_volume = audio_config.value("sound_volume", _sound_volume);
    }
    if(j.contains("input_mappings")&&j["input_mappings"].is_object()){
        const auto &mappings_json = j["input_mappings"];

        try{
            auto input_mappings = mappings_json.get<std::unordered_map<std::string,std::vector<std::string>>>();
            input_mappings = std::move(input_mappings);
            spdlog::info("Input mappings loaded from config file.");
        }catch(const std::exception& e){
            spdlog::warn("Failed to load input mappings from config file. Error: {}", e.what());
        }
    }else{
        spdlog::warn("Input mappings not found in config file.");
    }
}

nlohmann::ordered_json Config::toJson() const
{
    return nlohmann::ordered_json{
            {"window", {
                {"title", _window_title},
                {"width", _window_width},
                {"height", _window_height},
                {"resizable", _window_resizable}
            }},
            {"graphics", {
                {"vsync", _vsync_enabled}
            }},
            {"performance", {
                {"target_fps", _target_fps}
            }},
            {"audio", {
                {"music_volume", _music_volume},
                {"sound_volume", _sound_volume}
            }},
            {"input_mappings", _input_mappings}
        };
}

}