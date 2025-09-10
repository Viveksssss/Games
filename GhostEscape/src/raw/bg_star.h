#ifndef BG_STAR_H
#define BG_STAR_H

#include "../core/object.h"
#include <vector>

class BgStar:public Object{

protected:
    std::vector<glm::vec2>_star_far;
    std::vector<glm::vec2>_star_mid;
    std::vector<glm::vec2>_star_near;

    float _scale_far = 0.2;
    float _scale_mid = 0.5;
    float _scale_near = 0.7;

    SDL_FColor _color_far ={0,0,0,1};
    SDL_FColor _color_mid ={0,0,0,1};
    SDL_FColor _color_near ={0,0,0,1};

    int num = 2000; // 星星数量
    float timer = 0.0f;
public:
    static BgStar*create(Object*parent,int size,float scale_far,float scale_mid,float scale_near);
    virtual void update(float dt)override;
    virtual void render()override;

    // getter and setter
    inline void setScaleFar(float scale_far){ _scale_far = scale_far;}
    inline void setScaleMid(float scale_mid){ _scale_mid = scale_mid;}
    inline void setScaleMear(float scale_near){ _scale_near = scale_near;}
    inline void setColorFar(SDL_FColor color_far){ _color_far = color_far;}
    inline void setColorMid(SDL_FColor color_mid){ _color_mid = color_mid;}
    inline void setColorNear(SDL_FColor color_near){ _color_near = color_near;}
    inline void setNum(int num){ this->num = num;}
    inline int getNum(){ return this->num;}
    inline float getScaleFar(){ return _scale_far;}
    inline float getScaleMid(){ return _scale_mid;}
    inline float getScaleNear(){ return _scale_near;}




};




#endif