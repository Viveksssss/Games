#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL3/SDL_render.h>
#include <string>
#include <variant>
#include <vector>



namespace engine::core{
class Config;
}

namespace engine::input{

enum class ActionState{
    INACTIVE,
    PRESS_THIS_FRAME,
    HELD_DOWN,
    RELEASED_THIS_FRAME
};


class InputManager final{

private:
    SDL_Renderer*_renderer;
    // 实际按键           ->        抽象按键名称    <-          动作
    // _input_to_actions_map          ->        _actions_to_keyname_map

    /* 动作名称到按键名称 */
    std::unordered_map<std::string,std::vector<std::string>>_actions_to_keyname_map;

    /* 输入到关联的动作名称列表 */
    std::unordered_map<std::variant<Uint32,SDL_Scancode>,std::vector<std::string>>_input_to_actions_map;

    /* 每个动作的当前状态 */
    std::unordered_map<std::string,ActionState>_action_states;

    bool _should_quit = false;
    glm::vec2 _mouse_position;


public:
    InputManager(SDL_Renderer* renderer,const engine::core::Config*config);

    /* 更新输入状态，每轮优先调用 */
    void update();

    /* 动作检查 */
    bool isActionDown(const std::string&action_name) const;
    bool isActionPressed(const std::string&action_name) const;
    bool isActionReleased(const std::string&action_name) const;

    bool shouldQuit() const;
    void setShouldQuit(bool should_quit);

    glm::vec2 getMousePosition() const;
    glm::vec2 getLogicalMousePosition() const;

private:
    /* 处理事件，将按键转化为状态 */
    void processEvent(const SDL_Event&event);
    void initializeMappings(const engine::core::Config*config);

    void updateActionState(const std::string&action_name,bool is_input_active ,bool is_repeat_event);

    SDL_Scancode scancodeFromString(const std::string&key_name) const;
    Uint32 mouseButtonFromString(const std::string&button_name) const;
};


}