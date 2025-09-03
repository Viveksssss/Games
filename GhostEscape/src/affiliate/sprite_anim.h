#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H
#include "sprite.h"

class SpriteAnim : public Sprite {
protected:
    int _current_frame = 0;
    int _total_frame = 0;
    int _fps = 10;
    float _frame_timer = 0.0f;
    bool is_loop = true;
    bool is_finish = false;

public:
    inline int getCurrentFrame() const { return _current_frame; };
    inline void setCurrentFrame(int frame) { _current_frame = frame; };
    inline int getTotalFrame() const { return _total_frame; };
    inline void setTotalFrame(int total_frame) { _total_frame = total_frame; };
    inline int getFps() const { return _fps; };
    inline void setFps(int fps) { _fps = fps; };
    inline float getFrameTimer() const { return _frame_timer; };
    inline void setFrameTimer(float frame_timer) { _frame_timer = frame_timer; };
    inline bool getLoop() const { return is_loop; };
    inline void setLoop(bool loop) { is_loop = loop; };
    inline bool getFinish() const { return is_finish; };
    inline void setFinish(bool finish) { is_finish = finish; };

    virtual void update([[maybe_unused]] float dt) override;
    virtual void render() override;
    virtual void setTexture(const Texture& texture) override;

    static SpriteAnim* create(ObjectScreen* parent, const std::string& path, float scale = 1.0f, Anchor anchor = Anchor::CENTER);
};

#endif // SPRITE_ANIM_H