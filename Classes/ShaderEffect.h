//	Created by yuerain
//	shader特效基于sprite
#pragma once
#include "cocos2d.h"
USING_NS_CC;

/**
 *特效精灵
 */
class Effect;
class EffectSprite : public Sprite
{
public:
	EffectSprite();
	~EffectSprite();

	static EffectSprite* create(const std::string& filename);
	static EffectSprite* create(const std::string& filename, const cocos2d::Rect& rect);

	static EffectSprite* createWithTexture(cocos2d::Texture2D *texture);
	static EffectSprite* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated = false);

	static EffectSprite* createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
	static EffectSprite* createWithSpriteFrameName(const std::string& spriteFrameName);

	/**
	 *Effect是特效基类 要用其子类创建的对象
	 *@EffectGrey::create()			灰色
	 *@EffectOutline::create()		描边
	 *@EffectBlur::create()			模糊
	 *@EffectNoise::create()		噪音
	 *@EffectEdgeDetect::create()	边缘检测 画边
	 *@EffectSepia::create()		朦胧 墨色
	 *@EffectBloom::create()		闪亮
	 *@EffectCelShading::create()	卡通渲染
	 *@EffectLensFlare::create()	镜头光晕
	 */
	void setEffect(Effect* effect);
	
protected:
	bool initWithTexture(cocos2d::Texture2D *texture);
	bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
	virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated);
	virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
private:
	Effect* _defaultEffect;
};

/**
 *特效基类Effect
 */
class Effect : public Ref
{
public:
	Effect();
	virtual ~Effect();

	GLProgramState* getGLProgramState() const { return _glprogramstate; }
	virtual void setTarget(EffectSprite* sprite){}

protected:
	bool initGLProgramState(const std::string& fragmentFilename);
	GLProgramState* _glprogramstate;

private:
	
};

/**
 *Grey灰色
 */
class EffectGrey : public Effect
{
public:
	CREATE_FUNC(EffectGrey);
	bool init();
private:

};

/**
 *Outline描边
 */
class EffectOutline : public Effect
{
public:
	CREATE_FUNC(EffectOutline);
	bool init();
private:

};

/**
 *Blur模糊
 */
class EffectBlur : public Effect
{
public:
	CREATE_FUNC(EffectBlur);
	virtual void setTarget(EffectSprite *sprite) override;
	void setBlurRadius(float radius);
	void setBlurSampleNum(float num);

protected:
	bool init(float blurRadius = 10.0f, float sampleNum = 5.0f);

	float _blurRadius;
	float _blurSampleNum;
};

/**
 *Noise噪音
 */
class EffectNoise : public Effect
{
public:
	CREATE_FUNC(EffectNoise);

protected:
	bool init();
	virtual void setTarget(EffectSprite* sprite) override;
};

/** 
 *Edge Detect边缘检测 画边
 */
class EffectEdgeDetect : public Effect
{
public:
	CREATE_FUNC(EffectEdgeDetect);

protected:
	bool init();
	virtual void setTarget(EffectSprite* sprite) override;
};

/**
 *Sepia朦胧 墨色
 */
class EffectSepia : public Effect
{
public:
	CREATE_FUNC(EffectSepia);

protected:
	bool init();
};

/**
 *bloom闪亮
 */
class EffectBloom : public Effect
{
public:
	CREATE_FUNC(EffectBloom);

protected:
	bool init();
	virtual void setTarget(EffectSprite* sprite) override;
};

/**
 *cel shading卡通渲染
 */
class EffectCelShading : public Effect
{
public:
	CREATE_FUNC(EffectCelShading);

protected:
	bool init();
	virtual void setTarget(EffectSprite* sprite) override;
};

/**
 *Lens Flare镜头光晕
 */
class EffectLensFlare : public Effect
{
public:
	CREATE_FUNC(EffectLensFlare);

protected:
	bool init();
	virtual void setTarget(EffectSprite* sprite) override;
};