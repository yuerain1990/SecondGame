//	Created by yuerain
#include "ShaderEffect.h"

EffectSprite::EffectSprite() : _defaultEffect(nullptr)
{
}

EffectSprite::~EffectSprite()
{
	CC_SAFE_DELETE(_defaultEffect);
}

EffectSprite* EffectSprite::create(const std::string& filename)
{
	EffectSprite *sprite = new (std::nothrow) EffectSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

EffectSprite* EffectSprite::create(const std::string& filename, const cocos2d::Rect& rect)
{
	EffectSprite *sprite = new (std::nothrow) EffectSprite();
	if (sprite && sprite->initWithFile(filename, rect))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

EffectSprite* EffectSprite::createWithTexture(cocos2d::Texture2D *texture)
{
	EffectSprite *sprite = new (std::nothrow) EffectSprite();
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

EffectSprite* EffectSprite::createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated)
{
	EffectSprite *sprite = new (std::nothrow) EffectSprite();
	if (sprite && sprite->initWithTexture(texture, rect, rotated))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

EffectSprite* EffectSprite::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
	EffectSprite *sprite = new (std::nothrow) EffectSprite();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

EffectSprite* EffectSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
	cocos2d::SpriteFrame *frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	return createWithSpriteFrame(frame);
}

bool EffectSprite::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated)
{
	bool ret = Sprite::initWithTexture(texture, rect, rotated);
	if (ret)
	{
		return ret;
	}
	return false;
}

bool EffectSprite::initWithTexture(cocos2d::Texture2D *texture)
{
	CCASSERT(texture != nullptr, "Invalid texture for sprite");

	cocos2d::Rect rect = cocos2d::Rect::ZERO;
	rect.size = texture->getContentSize();

	return initWithTexture(texture, rect);
}

bool EffectSprite::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
	return initWithTexture(texture, rect, false);
}

bool EffectSprite::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
	CCASSERT(spriteFrame != nullptr, "");

	bool bRet = initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
	setSpriteFrame(spriteFrame);

	return bRet;
}

/**
 *设置特效
 */
void EffectSprite::setEffect(Effect* effect)
{
	if (_defaultEffect != effect){
		effect->setTarget(this);

		CC_SAFE_DELETE(_defaultEffect);
		_defaultEffect = effect;
		CC_SAFE_RETAIN(_defaultEffect);

		setGLProgramState(_defaultEffect->getGLProgramState());
	}
}

/**
 *特效基类Effect
 */
Effect::Effect()
{
}

Effect::~Effect()
{
	CC_SAFE_RELEASE_NULL(_glprogramstate);
}

bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
	auto fileUtiles = FileUtils::getInstance();
	auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
	auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());

	_glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	_glprogramstate->retain();

	return _glprogramstate != nullptr;
}

/**
 *Grey灰色
 */
bool EffectGrey::init()
{
	initGLProgramState("example_greyScale.fsh");

	return true;
}

/**
 *Outline描边
 */
bool EffectOutline::init()
{
	initGLProgramState("example_outline.fsh");

	Vec3 color(1.0f, 0.2f, 0.3f);
	GLfloat radius = 0.01f;
	GLfloat threshold = 1.75;

	_glprogramstate->setUniformVec3("u_outlineColor", color);
	_glprogramstate->setUniformFloat("u_radius", radius);
	_glprogramstate->setUniformFloat("u_threshold", threshold);

	return true;
}

/**
 *Blur模糊
 */
void EffectBlur::setTarget(EffectSprite *sprite)
{
	Size size = sprite->getTexture()->getContentSizeInPixels();
	_glprogramstate->setUniformVec2("resolution", size);
	_glprogramstate->setUniformFloat("blurRadius", _blurRadius);
	_glprogramstate->setUniformFloat("sampleNum", _blurSampleNum);
}

bool EffectBlur::init(float blurRadius, float sampleNum)
{
	initGLProgramState("example_Blur.fsh");
	_blurRadius = blurRadius;
	_blurSampleNum = sampleNum;

	return true;
}

void EffectBlur::setBlurRadius(float radius)
{
	_blurRadius = radius;
}

void EffectBlur::setBlurSampleNum(float num)
{
	_blurSampleNum = num;
}

/**
 *Noise噪音
 */
bool EffectNoise::init() 
{
	initGLProgramState("example_Noisy.fsh");
	return true;
}

void EffectNoise::setTarget(EffectSprite* sprite) 
{
	auto s = sprite->getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
}

/**
 *Edge Detect边缘检测 画边
 */
bool EffectEdgeDetect::init() 
{
	initGLProgramState("example_edgeDetection.fsh");
	return true;
}

void EffectEdgeDetect::setTarget(EffectSprite* sprite)
{
	auto s = sprite->getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
}

/**
 *Sepia朦胧 墨色
 */
bool EffectSepia::init()
{
	initGLProgramState("example_sepia.fsh");
	return true;
}

/**
 *bloom闪亮
 */
bool EffectBloom::init()
{
	initGLProgramState("example_bloom.fsh");
	return true;
}

void EffectBloom::setTarget(EffectSprite* sprite)
{
	auto s = sprite->getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
}

/**
 *cel shading卡通渲染
 */
bool EffectCelShading::init()
{
	initGLProgramState("example_celShading.fsh");
	return true;
}

void EffectCelShading::setTarget(EffectSprite* sprite)
{
	auto s = sprite->getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
}

/**
 *Lens Flare镜头光晕
 */
bool EffectLensFlare::init() {
	initGLProgramState("example_lensFlare.fsh");
	return true;
}

void EffectLensFlare::setTarget(EffectSprite* sprite)
{
	auto s = sprite->getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("textureResolution", Vec2(s.width, s.height));

	s = Director::getInstance()->getWinSize();
	getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));

}