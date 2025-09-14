#pragma once

namespace engine::input {
class InputManager;
}

namespace engine::render {
class Renderer;
class Camera;
}

namespace engine::resource {
class ResourceManager;
}

namespace engine::core {

/**
 * @brief 持有各个核心的上下文对象
 */
class Context final {

private:
    engine::input::InputManager& _inputManager;
    engine::render::Renderer& _renderer;
    engine::render::Camera& _camera;
    engine::resource::ResourceManager& _resourceManager;

public:
    /**
     * @brief 构造函数
     * @param input_manager 
     * @param renderer 
     * @param camera 
     * @param resource_manager 
     */
    Context(engine::input::InputManager&input_manager,engine::render::Renderer&renderer,engine::render::Camera&camera,engine::resource::ResourceManager&resource_manager);

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;

    // getter
    engine::input::InputManager&getInputManager()const {return _inputManager;};
    engine::render::Renderer&getRenderer()const {return _renderer;};
    engine::render::Camera&getCamera()const {return _camera;};
    engine::resource::ResourceManager&getResourceManager()const {return _resourceManager;};

};

}