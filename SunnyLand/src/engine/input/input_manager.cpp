#include "input_manager.h"
#include "../core/config.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::input {
InputManager::InputManager(SDL_Renderer* renderer, const engine::core::Config* config)
    : _renderer(renderer)
{
    if (!renderer) {
        spdlog::error("Renderer is null");
        throw std::runtime_error("Renderer is null");
    }
    initializeMappings(config);
    float x, y;
    SDL_GetMouseState(&x, &y);
    _mouse_position = { x, y };
    spdlog::debug("Mouse position: ({},{})", x, y);
}

void InputManager::update()
{
    for (auto& [action_name, state] : _action_states) {
        if (state == ActionState::PRESS_THIS_FRAME) {
            state = ActionState::HELD_DOWN;
        } else if (state == ActionState::RELEASED_THIS_FRAME) {
            state = ActionState::INACTIVE;
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        processEvent(event);
    }
}

bool InputManager::isActionDown(const std::string& action_name) const
{
    if (auto it = _action_states.find(action_name); it != _action_states.end()) {
        return it->second == ActionState::HELD_DOWN || it->second == ActionState::PRESS_THIS_FRAME;
    }
    return false;
}

bool InputManager::isActionPressed(const std::string& action_name) const
{
    if (auto it = _action_states.find(action_name); it != _action_states.end()) {
        return it->second == ActionState::PRESS_THIS_FRAME;
    }
    return false;
}

bool InputManager::isActionReleased(const std::string& action_name) const
{
    if (auto it = _action_states.find(action_name); it != _action_states.end()) {
        return it->second == ActionState::RELEASED_THIS_FRAME;
    }
    return false;
}

bool InputManager::shouldQuit() const
{
    return _should_quit;
}

void InputManager::setShouldQuit(bool should_quit)
{
    _should_quit = should_quit;
}

glm::vec2 InputManager::getMousePosition() const
{
    return _mouse_position;
}

glm::vec2 InputManager::getLogicalMousePosition() const
{
    glm::vec2 logical_pos;
    SDL_RenderCoordinatesFromWindow(_renderer, _mouse_position.x, _mouse_position.y, &logical_pos.x, &logical_pos.y);
    return logical_pos;
}

void InputManager::processEvent(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP: {
        SDL_Scancode scancode = event.key.scancode;
        bool is_down = event.key.down;
        bool is_repeat = event.key.repeat;
        if (auto it = _input_to_actions_map.find(scancode); it != _input_to_actions_map.end()) {
            const std::vector<std::string>& associated_actions = it->second;
            for (const std::string& action_name : associated_actions) {
                updateActionState(action_name, is_down, is_repeat);
            }
        }
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        Uint32 button = event.button.button;
        bool is_down = event.button.down;
        if (auto it = _input_to_actions_map.find(button); it != _input_to_actions_map.end()) {
            const std::vector<std::string>& associated_actions = it->second;
            for (const std::string& action_name : associated_actions) {
                updateActionState(action_name, is_down, false);
            }
        }
        _mouse_position = { event.button.x, event.button.y };
        break;
    }
    case SDL_EVENT_MOUSE_MOTION:
        _mouse_position = { event.motion.x, event.motion.y };
        break;
    case SDL_EVENT_QUIT:
        _should_quit = true;
        break;
    }
}

void InputManager::initializeMappings(const engine::core::Config* config)
{
    spdlog::debug("初始化映射...");
    if (!config) {
        spdlog::error("Config is null");
        throw std::runtime_error("Config is null");
    }

    /* 初始化输入映射 */
    _actions_to_keyname_map = config->_input_mappings;
    _input_to_actions_map.clear();
    _action_states.clear();

    if (_actions_to_keyname_map.find("MouseLeftClick") == _actions_to_keyname_map.end()) {
        spdlog::debug("没有找到鼠标左键映射");
        _actions_to_keyname_map["MouseLeftClick"] = { "MouseLeft" };
    }
    if (_actions_to_keyname_map.find("MouseRightClick") == _actions_to_keyname_map.end()) {
        spdlog::debug("没有找到鼠标右键映射");
        _actions_to_keyname_map["MouseRightClick"] = { "MouseRight" };
    }
    for (const auto& [action_name, key_names] : _actions_to_keyname_map) {
        _action_states[action_name] = ActionState::INACTIVE;
        // spdlog::debug("动作{}对应的按键：{}", action_name, key_names);
        /* 设置按键->动作 */

        for (const auto& key_name : key_names) {
            SDL_Scancode scancode = scancodeFromString(key_name);
            Uint32 mouse_button = mouseButtonFromString(key_name);
            if (scancode != SDL_SCANCODE_UNKNOWN) {
                _input_to_actions_map[scancode].push_back(action_name);
            } else if (mouse_button != 0) {
                _input_to_actions_map[mouse_button].push_back(action_name);
            } else {
                spdlog::warn("未知的按键名称：{}", key_name);
            }
        }
    }
    spdlog::debug("初始化映射完成");
}

void InputManager::updateActionState(const std::string& action_name, bool is_input_active, bool is_repeat_event)
{
    auto it = _action_states.find(action_name);
    if (it == _action_states.end()) {
        spdlog::warn("未知的动作名称：{}", action_name);
        return;
    }
    if (is_input_active) {
        if (is_repeat_event) {
            it->second = ActionState::HELD_DOWN;
        } else {
            it->second = ActionState::PRESS_THIS_FRAME;
        }
    } else {
        it->second = ActionState::RELEASED_THIS_FRAME;
    }
}

SDL_Scancode InputManager::scancodeFromString(const std::string& key_name) const
{
    return SDL_GetScancodeFromName(key_name.c_str());
}

Uint32 InputManager::mouseButtonFromString(const std::string& button_name) const
{
    if (button_name == "MouseLeft") {
        return SDL_BUTTON_LEFT;
    }
    if (button_name == "MouseRight") {
        return SDL_BUTTON_RIGHT;
    }
    if (button_name == "MouseMiddle") {
        return SDL_BUTTON_MIDDLE;
    }
    if (button_name == "MouseX1") {
        return SDL_BUTTON_X1;
    }
    if (button_name == "MouseX2") {
        return SDL_BUTTON_X2;
    }
    return 0;
}
}