#pragma once

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace engine::core {

class Config final {

public:
    std::string _window_title = "SunnyLand";
    int _window_width = 1920;
    int _window_height = 1080;
    bool _window_resizable = true;

    /* 垂直同步 */
    bool _vsync_enabled = true;

    int _target_fps = 240;

    float _music_volume = 0.5f;
    float _sound_volume = 0.5f;

    std::unordered_map<std::string, std::vector<std::string>> _input_mappings = {
        { "move_left", { "A", "Left" } },
        { "move_right", { "D", "Right" } },
        { "move_up", { "W", "Up" } },
        { "move_down", { "S", "Down" } },
        { "jump", { "J", "Ctrl" } },
        { "attack", { "K", "MouseLeft" } },
        { "pause", { "p", "Escape" } }
    };

    explicit Config(const std::string& filename);

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

    bool loadFromFile(const std::string& filename);
    [[nodiscard]] bool saveToFile(const std::string& filename) const;

private:
    void fromJson(const nlohmann::json& json);
    nlohmann::ordered_json toJson() const;
};

}