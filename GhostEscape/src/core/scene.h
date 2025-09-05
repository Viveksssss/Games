#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "object_screen.h"
#include "object_world.h"
#include <glm/glm.hpp>

class Scene : public Object {
protected:
    // 相机位置
    glm::vec2 _camera_position = glm::vec2(0);
    // 世界大小
    glm::vec2 _world_size = glm::vec2(0);
    // 世界物体
    std::vector<ObjectWorld*> _children_world;
    // 屏幕物体
    std::vector<ObjectScreen*> _children_screen;

public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void init() override { };
    // 循环遍历处理子对象：自动装载
    virtual void update([[maybe_unused]] float dt) override;
    virtual void render() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void clean() override;

    inline glm::vec2 worldToScreen(const glm::vec2& world_position) { return world_position - _camera_position; }
    inline glm::vec2 screenToWorld(const glm::vec2& screen_position) { return screen_position + _camera_position; }
    inline glm::vec2 getCameraPosition() const { return _camera_position; };
    inline glm::vec2 getWorldSize() const { return _world_size; };
    inline void setWorldSize(const glm::vec2& size) { _world_size = size; };

    // 根据类型添加子对象到不同的容器
    virtual void addChild(Object* child) override;
    // 删除子对象
    virtual void removeChild(Object* child) override;
    // 设置摄像机位置
    void setCameraPosition(const glm::vec2& position);
    // 获取场景中的物体
    inline std::vector<ObjectWorld*>& getChildrenObjectWorld() { return _children_world; }
    // 获取场景中的物体
    inline std::vector<ObjectScreen*>& getChildrenObjectScreen() { return _children_screen; }   
};

#endif