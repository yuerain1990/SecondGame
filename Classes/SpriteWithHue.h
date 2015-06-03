//
//  SpriteWithHue.h
//  
//
//  精灵颜色换装 修改色调实现资源复用
//  yuerain 15/6/1
//
//

#ifndef _SPRITE_WITH_HUE_
#define _SPRITE_WITH_HUE_

#include "cocos2d.h"

class SpriteWithHue : public cocos2d::Sprite
{
public:
    static SpriteWithHue* create(const std::string& filename);
    static SpriteWithHue* create(const std::string& filename, const cocos2d::Rect& rect);
    
    static SpriteWithHue* createWithTexture(cocos2d::Texture2D *texture);
    static SpriteWithHue* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated=false);
    
    static SpriteWithHue* createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
    static SpriteWithHue* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    float getHue();
	/**
	 *@parma hue范围是0-2*M_PI
	 */
    void setHue(float hue);
    
protected:
    float _hue;
    
    bool initWithTexture(cocos2d::Texture2D *texture);
    bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
    virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated);
    virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
    
    void setupDefaultSettings();
    void initShader();
    const GLchar* shaderBody();
    virtual void updateColor();
    void updateColorMatrix();
	void matCallback(cocos2d::GLProgram *p, cocos2d::Uniform *u);
    void updateAlpha();
    GLfloat getAlpha();
};

#endif /* defined(_SPRITE_WITH_HUE_) */
